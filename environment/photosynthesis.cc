#include "photosynthesis.h"

#include <algorithm>
#include <cmath>

namespace environment {

Photosynthesis::Photosynthesis(const Meteorology &meteorology,
                               const PlantRadiation &plant_radiation,
                               const EnergyBalance &energy_balance)
    : meteorology_(meteorology),
      plant_radiation_(plant_radiation),
      energy_balance_(energy_balance) {}

double Photosynthesis::Q10TemperatureSensitivity(
    double michaelis_menten_constant_c25, double q10_temperature_sensitivity,
    double leaf_temperature) {
  return (michaelis_menten_constant_c25 *
          pow(q10_temperature_sensitivity,
              (leaf_temperature - kTemperatureScalar25) / 10.0));
}

double Photosynthesis::RubiscoCapacityRate(double leaf_temperature) {
  // From section 6.4.1 on page 129
  double expression = 1.0 + exp(0.128 * (leaf_temperature - 40.0));
  return Q10TemperatureSensitivity(kRubiscoCapacityRate, 2.4,
                                   leaf_temperature) /
         expression;
}

double Photosynthesis::CO2CompensationPoint(double leaf_temperature) {
  // From formula 6.19 on page 132
  double sensitivity =
      Q10TemperatureSensitivity(kSpecificityFactor, 0.57, leaf_temperature);
  return (kO2ConcentrationInAir * 0.5 / sensitivity);
}

double Photosynthesis::PhotosynthesisLimitedByLight(double par,
                                                    double leaf_temperature,
                                                    double co2_concentration) {
  // From formula 6.25 on page 133
  double co2_compensation_point = CO2CompensationPoint(leaf_temperature);
  double numerator = kAbsorptionFraction * kQuantumYield * par *
                     (co2_concentration - co2_compensation_point);
  double denominator = co2_concentration + 2.0 * co2_compensation_point;
  return (numerator / denominator);
}

double Photosynthesis::PhotosynthesisLimitedByRubisco(
    double leaf_temperature, double co2_concentration) {
  // From formula 6.15 on page 129
  double michaelis_menten_co2 = Q10TemperatureSensitivity(
      kMichaelisMentenCo2Degrees25, 2.1, leaf_temperature);
  double michaelis_menten_o2 = Q10TemperatureSensitivity(
      kMichaelisMentenO2Degrees25, 1.2, leaf_temperature);
  double dKm = michaelis_menten_co2 *
               (1.0 + kO2ConcentrationInAir / michaelis_menten_o2);
  double numerator =
      RubiscoCapacityRate(leaf_temperature) *
      (co2_concentration - CO2CompensationPoint(leaf_temperature));
  double denominator = co2_concentration + dKm;
  return (numerator / denominator);
}

double Photosynthesis::PhotosynthesisLimitedBySucroseSink(
    double leaf_temperature) {
  // From formula 6.26 on page 134
  return (0.5 * RubiscoCapacityRate(leaf_temperature));
}

double Photosynthesis::LeafPhotosynthesis(double par, double leaf_temperature) {
  // From formula 6.27 on page 134
  double rubisco_limited_value = PhotosynthesisLimitedByRubisco(
      leaf_temperature, kInternalCo2Concentration);
  double light_limited_value = PhotosynthesisLimitedByLight(
      par, leaf_temperature, kInternalCo2Concentration);
  double sucrose_sink_limited_value =
      PhotosynthesisLimitedBySucroseSink(leaf_temperature);

  // find the most limiting factor to assimilation
  return fmin(rubisco_limited_value,
              fmin(light_limited_value, sucrose_sink_limited_value));
}

double Photosynthesis::GrossCanopyPhotosynthesis(double local_solar_hour,
                                                 double leaf_temperature) {
  PlantRadiation::AbsorbedPhotosyntheticallyActiveRadiation
      absorbedHourParReturn = plant_radiation_.CalculateAbsorbedHourPAR(
          0, 0,
          0);  // TODO: Figure Out What Values to put here as parameters!!!!
  double sunlit = absorbedHourParReturn.sunlit * kConversionFactorToUmolM2S1;
  double shaded = absorbedHourParReturn.shaded * kConversionFactorToUmolM2S1;
  // leaf assimilation:
  double photosynthesis_sunlit = LeafPhotosynthesis(sunlit, leaf_temperature);
  double photosynthesis_shaded = LeafPhotosynthesis(shaded, leaf_temperature);
  // sunlit and shaded LAI
  PlantRadiation::LeafIndexArea leafIndexAreaReturn =
      plant_radiation_.CalculateLai(
          local_solar_hour);  // TODO: Is this the right input value to the
                              // function???
                              
  // From formula 6.36 on page 138
  return (photosynthesis_sunlit * leafIndexAreaReturn.sunlit +
          photosynthesis_shaded *
              leafIndexAreaReturn.shaded);  // in umol CO2 m-2 s-1
}

double Photosynthesis::DailyGrossCanopyPhotosynthesis() {
  // 5-point gauss integration over sunrise to sunset:
  double const ABS[5] = {0.0469101, 0.2307534, 0.5000000, 0.7692465, 0.9530899};
  double const WGT[5] = {0.1184635, 0.2393144, 0.2844444, 0.2393144, 0.1184635};

  // From formula 6.37 on page 140
  double solar_hour_sunrise = meteorology_.solar_hour_sunrise();
  double solar_hours_per_day =
      meteorology_.solar_hour_sunset() - solar_hour_sunrise;
  double assimilation = 0.0;

  for (int i = 0; i < 5; ++i) {
    double solar_hour = solar_hour_sunrise + ABS[i] * solar_hours_per_day;
    double total_sensible_heat_flux =
        energy_balance_.total_sensible_heat_flux();
    double sensible_heat_flux_crop = energy_balance_.sensible_heat_flux_crop();

    // TODO: should energy balance be defined? How should
    // CalculateCanopyTemperature be called?
    double canopy_temperature = energy_balance_.CalculateCanopyTemperature(
        solar_hour, total_sensible_heat_flux, sensible_heat_flux_crop);
    double gross_canopy_photosynthesis =
        GrossCanopyPhotosynthesis(solar_hour, canopy_temperature);
    assimilation += gross_canopy_photosynthesis * kHourToSeconds * WGT[i] *
                    solar_hours_per_day;
  }

  return assimilation;
}

}  // namespace environment