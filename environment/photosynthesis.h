#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PHOTOSYNTHESIS_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PHOTOSYNTHESIS_H_

namespace environment {

class Photosynthesis {

   // Changes the given model parameter based on current temperature.
   //   k25 (model parameter at 25 deg. C): how well a substrate complexes with a given enzyme
   //   q10_temperature_sensitivity (Q10 value): a measure of the temperature sensitivity of an enzymatic reaction rate due to increase in temperature by 10 degrees celsisu,
   //   leaf_temperature (leaf temperature, deg. C)
   double Q10TemperatureSensitivity(double michaelis_menten_constant_25_degrees, double q10_temperature_sensitivity, double leaf_temperature);

   // Rubisco capacity rate (umol m-2 s-1)
   //   lftemp (leaf temperature, deg. C)
   double RubiscoCapacityRate(double leaf_temperature);

   // CO2 compensation point (umol mol-1)
   //   lftemp (leaf temperature, deg. C)
   double CO2CompensationPoint(double leaf_temperature);

   // Gross photosynthesis limited by light (umol m-2 s-1)
   //   par (PAR, umol m-2 s-1), lftemp (leaf temperature, deg. C),
   //   ci (intercellular CO2 concentration, umol mol-1)
   double PhotosynthesisLimitedByLight(double par, double leaf_temperature, double co2_concentration);

   // Gross photosynthesis limited by Rubisco capacity (umol m-2 s-1)
   //   lftemp (leaf temperature, deg. C),
   //   ci (intercellular CO2 concentration, umol mol-1)
   double PhotosynthesisLimitedByRubisco(double leaf_temperature, double co2_concentration);

   // Gross photosynthesis limited by sucrose sink (umol m-2 s-1)
   //   lftemp (leaf temperature, deg. C)
   double PhotosynthesisLimitedBySucroseSink(double leaf_temperature);

   // Gross leaf photosynthesis (umol m-2 s-1)
   //   par (PAR, umol m-2 s-1), lftemp (leaf temperature, deg. C),
   double LeafPhotosynthesis(double par, double leaf_temperature);

   // Gross canopy photosynthesis (per unit ground area)
   //   (umol CO2 m-2 s-1).
   //   th (local solar time, hour), lai (leaf area index, m2 m-2),
   //   lftemp (leaf temperature, deg. C)
   double GrossCanopyPhotosynthesis(double local_solar_hour, double leaf_temperature);

   // Daily gross canopy photosynthesis (per unit ground area)
   //   (umol CO2 m-2 day-1)
   double DailyGrossCanopyPhotosynthesis();


   static constexpr double rubisco_capacity_rate = 200.0;
   static constexpr double specificity_factor = 2600.0;
   static constexpr double o2_concentration_in_air = 210000.0;
   static constexpr double quantum_yield = 0.06;
   static constexpr double absorption_fraction = 0.8;
   static constexpr double michaelis_menten_co2_25_degrees = 300.0;    // michaelis menten constant for co2 at 25 degrees celsius
   static constexpr double michaelis_menten_o2_25_degrees = 300000.0; // michaelis menten constant for o2 at 25 degrees celsius
   static constexpr double internal_co2_concentration = 245.0; // (umol mol-1)

   static constexpr double conversion_factor_to_umol_m2_s1 = 4.5;
};

}  // namespace environment

#endif COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PHOTOSYNTHESIS_H_
