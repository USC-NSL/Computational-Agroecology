#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WATER_BALANCE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WATER_BALANCE_H_

#include <utility>

namespace environment {

class WaterBalance {
 public:
  // This struct returns the amount of water in both layers of soil.
  //   For modelling purposes, the soil is treated as having only two layers
  //   which interact with each other, and hold separate contents.
  struct DailyWaterContentReturn {
    double water_amount_1;
    double water_amount_2;
  };

  // Final volumetric water content (m3 m-3) for the two soil layers. (mm
  // day-1).
  static DailyWaterContentReturn DailyWaterContent(double rainfall,
                                                   double water_amount_1,
                                                   double water_amount_2,
                                                   double dETs, double dETc);

  // Fraction of growth reduced due to limited water. potT is potential
  //   transpiration (mm day-1).
  static double GrowthReduction(double potential_transpiration,
                                double water_amount_2);

 private:
  // The purpose of making the constructor private is to prevent initialization
  // of the class, because all functions in this class are static
  WaterBalance() {}

  // This struct describes the amount of water that is evaporated from each
  //   layer of the soil, since depths affects how much surface area is exposed.
  //   Actual evaporation is calculated by multiplying the potential evaporation
  //   by some reduction factor.
  struct ActualEvaporationReturn {
    double layer_1;
    double layer_2;
  };

  // This struct describes the amount of water that is transpired from each
  //   layer of the soil, since depths affects how much surface area is exposed.
  //   Transpiration is the process of water loss from plants through stomata.
  struct ActualTranspirationReturn {
    double layer_1;
    double layer_2;
  };

  struct VolumetricWaterContentReturn {
    double volumetric_water_content;
    double percolation_to_below;
  };

  // Water content before redistribution (mm)
  //   soil_layer_thickness = soil layer thickness (m)
  //   water_content = current water content (m3 m-3)
  //   percolation_from_above = water percolating from the above soil layer (mm
  //   day-1)
  static double WaterContentBeforeRedistribution(double soil_layer_thickness,
                                                 double water_content,
                                                 double percolation_from_above);

  // Water content after redistribution (mm)
  //   soil_thickness = soil layer thickness (m)
  //   water_content = current water content (m3 m-3)
  static double WaterContentAfterRedistribution(double soil_thickness,
                                                double water_content);

  // Actual soil evaporation (mm day-1)
  //   potential_evaporation = potential soil evaporation (mm day-1)
  static ActualEvaporationReturn ActualEvaporation(double potential_evaporation,
                                                   double water_amount_1);

  // Actual plant transpiration (mm day-1)
  //   photosynthesis_efficiency = 0.5 or 0.3 for C3 and C4 plants,
  //   respectively, represents photosynthesis efficiency
  //   potential_transpiration = potential soil transpiration (mm day-1)
  static ActualTranspirationReturn ActualTranspiration(
      double photosynthesis_efficiency, double potential_transpiration,
      double water_amount_2);

  // Final volumetric water content (m3 m-3)
  //   soil_thickness = soil layer thickness (m)
  //   volumetric_water_content = current volumetric water content (m3 m-3)
  //   actual_evaporation and actual_transpiration = actual evaporation and
  //   transpiration (mm day-1) percolation_from_above = percolation from above
  //   (mm day-1)
  static VolumetricWaterContentReturn VolumetricWaterContent(
      double soil_thickness, double volumetric_water_content,
      double actual_evaporation, double actual_transpiration,
      double percolation_from_above);

  // Values based on "input.txt" file from original code
  static constexpr double saturation_point = 0.39;
  static constexpr double wilting_point = 0.07;
  static constexpr double hydraulic_slope = 14.5;
  static constexpr double saturated_hydraulic = 0.19;
  static constexpr int interval = 5;
  static constexpr double depth_1 = 0.02;
  static constexpr double depth_2 = 0.03;
  static constexpr double photosynthesis_efficiency_c3 = 0.05;
  // Explained on page 71, it's the amount of energy required to convert 1 kg of
  // liquid water to vapor, without any change in temperature (joules)
  static constexpr double latent_heat_of_vaporization_of_water = 2454000.0;
  static constexpr double liquid_density_for_water_20c = 998.0;
  static constexpr double portential_factor =
      1000.0 /
      (latent_heat_of_vaporization_of_water * liquid_density_for_water_20c);

  static constexpr int kMeterToMillimeter = 1000;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WATER_BALANCE_H_