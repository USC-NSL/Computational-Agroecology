#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WATER_BALANCE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WATER_BALANCE_H_

#include <utility>

namespace environment {

class WaterBalance {

  public:

    struct DailyWaterContentReturn {
        double water_amount_1;
        double water_amount_1;
    };
    
    // Final volumetric water content (m3 m-3) for the two soil layers. (mm day-1).
    DailyWaterContentReturn DailyWaterContent(double rainfall, double water_amount_1, double water_amount_2, double dETs, double dETc);

    // Fraction of growth reduced due to limited water. potT is potential
    //   transpiration (mm day-1).
    double GrowthReduction(double potential_transpiration, double water_amount_2);

  private:

    struct ActualEvaporationReturn {
        double layer_1;
        double layer_2;
    };

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
    //   percolation_from_above = water percolating from the above soil layer (mm day-1)
    double WaterContentBeforeRedistribution(double soil_layer_thickness, double water_content, double percolation_from_above);

    // Water content after redistribution (mm)
    //   soil_thickness = soil layer thickness (m)
    //   water_content = current water content (m3 m-3)
    double WaterContentAfterRedistribution(double soil_thickness, double water_content);

    // Actual soil evaporation (mm day-1)
    //   potential_evaporation = potential soil evaporation (mm day-1)
    ActualEvaporationReturn ActualEvaporation(double potential_evaporation, double water_amount_1);

    // Actual plant transpiration (mm day-1)
    //   photosynthesis_efficiency = 0.5 or 0.3 for C3 and C4 plants, respectively, represents photosynthesis efficiency
    //   potential_transpiration = potential soil transpiration (mm day-1)
    ActualTranspirationReturn ActualTranspiration(
        double photosynthesis_efficiency, double potential_transpiration, double water_amount_2);

    // Final volumetric water content (m3 m-3)
    //   soil_thickness = soil layer thickness (m)
    //   volumetric_water_content = current volumetric water content (m3 m-3)
    //   actual_evaporation and actual_transpiration = actual evaporation and transpiration (mm day-1)
    //   percolation_from_above = percolation from above (mm day-1)
    VolumetricWaterContentReturn VolumetricWaterContent(double soil_thickness, double volumetric_water_content, 
            double actual_evaporation, double actual_transpiration, double percolation_from_above);

    // Values based on "input.txt" file from original code
    static constexpr double saturation_point = 0.39;
    static constexpr const double wilting_point = 0.07;
    static constexpr const double hydraulic_slope = 14.5;
    static constexpr const double saturated_hydraulic = 0.19;
    static constexpr const int interval = 5;
    static constexpr const double depth_1 = 0.02;
    static constexpr const double depth_2 = 0.03;
    static constexpr const double photosynthesis_efficiency_c3 = 0.05;
    static constexpr const double portential_factor = 1000.0 / (2454000.0 * 998.0);

    static constexpr int kMeterToMillimeter = 1000;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WATER_BALANCE_H_