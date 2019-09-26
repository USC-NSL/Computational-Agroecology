#ifndef COMPUTATIONAL_AGROECOLOGY_WATER_BALANCE_H_
#define COMPUTATIONAL_AGROECOLOGY_WATER_BALANCE_H_

#include <utility>


namespace environment {

class WaterBalance {

    // Water content before redistribution (mm)
    //   length = soil layer thickness (m)
    //   wc = current water content (m3 m-3)
    //   wcperc = water percolating from the above soil layer (mm day-1)
    double WaterContentBeforeRedistribution(double soil_layer_thickness, double water_content, double percolation_from_above);

    // Water content after redistribution (mm)
    //   length = soil layer thickness (m)
    //   wc = current water content (m3 m-3)
    double WaterContentAfterRedistribution(double soil_thickness, double water_content);

    // Actual soil evaporation (mm day-1)
    //   potE = potential soil evaporation (mm day-1)
    //   e01 and e02 = actual evaporation from layer 1 and 2 (mm day-1)
    //                 (set within this function)
    void ActualEvaporation(double potential_evaporation, double &actual_evaporation_layer_1, double &actual_evaporation_layer_2);

    // Actual plant transpiration (mm day-1)
    //   p = 0.5 or 0.3 for C3 and C4 plants, respectively, represents photosynthesis efficiency
    //   potT = potential soil transpiration (mm day-1)
    //   t01 and t02 = actual transpiration from layer 1 and 2 (mm day-1)
    //                 (set within this function)
    void ActualTranspiration(double p, double potential_transpiration, 
            double &actual_transpiration_layer_1, double &actual_transpiration_layer_2);

    // Final volumetric water content (m3 m-3)
    //   length = soil layer thickness (m)
    //   vwc = current volumetric water content (m3 m-3)
    //   ea and ta = actual evaporation and transpiration (mm day-1)
    //   wcperc0 and wcperc1 = percolation from above and to below
    //                         (mm day-1)
    //   Note: wcperc1 is set within this function.
    double VolumetricWaterContent(double soil_thickness, double volumetric_water_content, 
            double actual_evaporation, double actual_transpiration, double percolation_from_above, double &percolation_to_below);

    // Final volumetric water content (m3 m-3) for the two soil
    //   layers. potE and potT are the potential evaporation and
    //   transpiration (mm day-1).
    void DailyWaterContent();

    // Fraction of growth reduced due to limited water. potT is potential
    //   transpiration (mm day-1).
    double GrowthReduction(double potential_transpiration);


    // TODO: Get dETs and dETc from "meteorology.cc" file, from UpdateHourlyNetRadiation (or maybe some other function)
    double dETs;
    double dETc;
    // TODO: Figure out how weather, and rain specifically, is defined in the project
    double rainfall; // Rainfall on current day (mm)

    // Values based on "input.txt" file from original code
    const double saturation_point = 0.39;
    const double wilting_point = 0.07;
    const double hydraulic_slope = 14.5;
    const double saturated_hydraulic = 0.19;
    const int interval = 5;
    const double depth_1 = 0.02;
    const double depth_2 = 0.03;
    double water_amount_1 = 0.2;
    double water_amount_2 = 0.2;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_WATER_BALANCE_H_