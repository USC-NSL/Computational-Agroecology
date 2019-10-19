#include "photosynthesis.h"
#include "plant_radiation.h"

#include <algorithm>
#include <cmath>

namespace environment {

double Photosynthesis::Q10TemperatureSensitivity(double michaelis_menten_constant_c25, double q10_temperature_sensitivity, double leaf_temperature)
{
    return (michaelis_menten_constant_c25 * pow(q10_temperature_sensitivity, (leaf_temperature - 25.0) / 10.0));
}

double Photosynthesis::RubiscoCapacityRate(double leaf_temperature)
{
    // From section 6.4.1 on page 129
    double expression = 1.0 + exp(0.128 * (leaf_temperature - 40.0));
    return Q10TemperatureSensitivity(rubisco_capacity_rate, 2.4, leaf_temperature) / expression;
}

double Photosynthesis::CO2CompensationPoint(double leaf_temperature)
{
    // From formula 6.19 on page 132
    double sensitivity = Q10TemperatureSensitivity(specificity_factor, 0.57, leaf_temperature);
    return (o2_concentration_in_air * 0.5 / sensitivity);
}

double Photosynthesis::PhotosynthesisLimitedByLight(double par, double leaf_temperature, double co2_concentration)
{
    // From formula 6.25 on page 133
    double co2_compensation_point = CO2CompensationPoint(leaf_temperature);
    double numerator = absorption_fraction * quantum_yield * par * (co2_concentration - co2_compensation_point);
    double denominator = co2_concentration + 2.0 * co2_compensation_point;
    return (numerator / denominator);
}

double Photosynthesis::PhotosynthesisLimitedByRubisco(double leaf_temperature, double co2_concentration)
{
    // From formula 6.15 on page 129
    double michaelis_menten_co2 = Q10TemperatureSensitivity(michaelis_menten_co2_25_degrees, 2.1, leaf_temperature);
    double michaelis_menten_o2 = Q10TemperatureSensitivity(michaelis_menten_o2_25_degrees, 1.2, leaf_temperature);
    double dKm = michaelis_menten_co2 * (1.0 + o2_concentration_in_air / michaelis_menten_o2);
    double numerator = RubiscoCapacityRate(leaf_temperature) * (co2_concentration - CO2CompensationPoint(leaf_temperature));
    double denominator = co2_concentration + dKm;
    return (numerator / denominator);
}

double Photosynthesis::PhotosynthesisLimitedBySucroseSink(double leaf_temperature)
{
    // From formula 6.26 on page 134
    return (0.5 * RubiscoCapacityRate(leaf_temperature));
}

double Photosynthesis::LeafPhotosynthesis(double par, double leaf_temperature)
{
    double rubisco_limited_value = PhotosynthesisLimitedByRubisco(leaf_temperature, internal_co2_concentration);
    double light_limited_value = PhotosynthesisLimitedByLight(par, leaf_temperature, internal_co2_concentration);
    double sucrose_sink_limited_value = PhotosynthesisLimitedBySucroseSink(leaf_temperature);

    // find the most limiting factor to assimilation
    return fmin(rubisco_limited_value, fmin(light_limited_value, sucrose_sink_limited_value));
}

double Photosynthesis::GrossCanopyPhotosynthesis(double local_solar_hour, double leaf_temperature)
{
    PlantRadiation::AbsorbedPhotosyntheticallyActiveRadiation absorbedHourParReturn = 
        PlantRadiation::CalculateAbsorbedHourPAR(0, 0, 0);  // TODO: Figure Out What Values to put here as parameters!!!!
    double sunlit = absorbedHourParReturn.sunlit * conversion_factor_to_umol_m2_s1;
    double shaded = absorbedHourParReturn.shaded * conversion_factor_to_umol_m2_s1;
    // leaf assimilation:
    double photosynthesis_sunlit = LeafPhotosynthesis(sunlit, leaf_temperature);
    double photosynthesis_shaded = LeafPhotosynthesis(shaded, leaf_temperature);
    // sunlit and shaded LAI
    double dLsl = 0.0, dLsh = 0.0;
    LAI(local_solar_hour, dLsl, dLsh);
    return (photosynthesis_sunlit * dLsl + photosynthesis_shaded * dLsh); // in umol CO2 m-2 s-1
}

double Photosynthesis::DailyGrossCanopyPhotosynthesis()
{
    // 5-point gauss integration over sunrise to sunset:
    double const ABS[5] = {0.0469101, 0.2307534, 0.5000000,
                        0.7692465, 0.9530899};
    double const WGT[5] = {0.1184635, 0.2393144, 0.2844444,
                        0.2393144, 0.1184635};

    double dTsr = Sunrise();
    double dIval = Sunset() - dTsr;
    double dAssim = 0.0;
    for (int i=0; i<5; ++i)
    {
        double dHour = dTsr + ABS[i] * dIval;   // current hour
        double dET = 0.0, dETs = 0.0, dETc = 0.0;
        double dH = 0.0, dHs = 0.0, dHc = 0.0;
        HourHeatFluxes(dHour, dET, dETs, dETc, dH, dHs, dHc);
        double dTf = CanopyTemp(dHour, dH, dHc);
        double dAn = GrossCanopyPhotosynthesis(dHour, dTf);
        // x 3600 to convert sec to hour:
        dAssim = dAssim + dAn * 3600.0 * WGT[i] * dIval;
    }

    return dAssim;
}

}  // namespace environment