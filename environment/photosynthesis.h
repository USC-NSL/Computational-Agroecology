#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PHOTOSYNTHESIS_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PHOTOSYNTHESIS_H_

namespace environment {

class Photosynthesis {
 public:
  // Gross canopy photosynthesis (per unit ground area)
  //   (umol CO2 m-2 s-1).
  //   th (local solar time, hour), lai (leaf area index, m2 m-2),
  //   lftemp (leaf temperature, deg. C)
  static double GrossCanopyPhotosynthesis(double local_solar_hour,
                                          double leaf_temperature);

  // Daily gross canopy photosynthesis (per unit ground area)
  //   (umol CO2 m-2 day-1)
  static double DailyGrossCanopyPhotosynthesis();

 private:
  // Changes the given model parameter based on current temperature.
  //   k25 (model parameter at 25 deg. C): how well a substrate complexes with a
  //   given enzyme q10_temperature_sensitivity (Q10 value): a measure of the
  //   temperature sensitivity of an enzymatic reaction rate due to increase in
  //   temperature by 10 degrees celsisu, leaf_temperature (leaf temperature,
  //   deg. C)
  static double Q10TemperatureSensitivity(
      double michaelis_menten_constant_25_degrees,
      double q10_temperature_sensitivity, double leaf_temperature);

  // Rubisco capacity rate (umol m-2 s-1)
  //   lftemp (leaf temperature, deg. C)
  static double RubiscoCapacityRate(double leaf_temperature);

  // CO2 compensation point (umol mol-1)
  //   lftemp (leaf temperature, deg. C)
  static double CO2CompensationPoint(double leaf_temperature);

  // Gross photosynthesis limited by light (umol m-2 s-1)
  //   par (PAR, umol m-2 s-1), lftemp (leaf temperature, deg. C),
  //   ci (intercellular CO2 concentration, umol mol-1)
  static double PhotosynthesisLimitedByLight(double par,
                                             double leaf_temperature,
                                             double co2_concentration);

  // Gross photosynthesis limited by Rubisco capacity (umol m-2 s-1)
  //   lftemp (leaf temperature, deg. C),
  //   ci (intercellular CO2 concentration, umol mol-1)
  static double PhotosynthesisLimitedByRubisco(double leaf_temperature,
                                               double co2_concentration);

  // Gross photosynthesis limited by sucrose sink (umol m-2 s-1)
  //   lftemp (leaf temperature, deg. C)
  static double PhotosynthesisLimitedBySucroseSink(double leaf_temperature);

  // Gross leaf photosynthesis (umol m-2 s-1)
  //   par (PAR, umol m-2 s-1), lftemp (leaf temperature, deg. C),
  static double LeafPhotosynthesis(double par, double leaf_temperature);

  static constexpr double kRubiscoCapacityRate = 200.0;
  static constexpr double kSpecificityFactor = 2600.0;
  static constexpr double kO2ConcentrationInAir = 210000.0;
  static constexpr double kQuantumYield = 0.06;
  static constexpr double kAbsorptionFraction = 0.8;
  static constexpr double kMichaelisMentenCo2Degrees25 =
      300.0;  // michaelis menten constant for co2 at 25 degrees celsius
  static constexpr double kMichaelisMentenO2Degrees25 =
      300000.0;  // michaelis menten constant for o2 at 25 degrees celsius
  static constexpr double kInternalCo2Concentration = 245.0;  // (umol mol-1)

  static constexpr double kConversionFactorToUmolM2S1 = 4.5;
  static constexpr double kTemperatureScalar25 = 25.0;
};

}  // namespace environment

#endif COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PHOTOSYNTHESIS_H_
