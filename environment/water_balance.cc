// header file: watbal.h
#ifndef WATBAL_H
#define WATBAL_H

#include "water_balance.h"
#include <cmath>

namespace environment {

double WaterBalance::WaterContentBeforeRedistribution(double soil_layer_thickness, double water_content, double percolation_from_above)
{
   // calculates point at which the water can no longer hold more water
   double water_content_saturation = saturation_point * soil_layer_thickness * 1000; // for conversion to mm
   double percolation_to_below = 0.0;
   double total_water_store = water_content + percolation_from_above;
   // Percolate water below if the current layer is too saturated
   if (total_water_store > water_content_saturation) {
      percolation_to_below = total_water_store - water_content_saturation;
   }
   return(water_content + percolation_from_above - percolation_to_below);
}

double WaterBalance::WaterContentAfterRedistribution(double soil_thickness, double water_content)
{
   double soil_thickness_mm = 1000.0 * soil_thickness;  // for conversion
   // From formula 5.3 on p. 107
   // Volumetric WC = volume of water contained in a unit volume of soil
   double volumetric_water_content = water_content / soil_thickness_mm;        // want unit in m3 m-3
   // From formula 5.20 on p. 114, used to calculate new water content in soil
   double exp_expression = exp((hydraulic_slope / saturation_point) *
                     (saturation_point - volumetric_water_content));
   double val_expression = (hydraulic_slope * saturated_hydraulic * interval) /
                 (soil_thickness * saturation_point);
   double log_expression = log(val_expression + exp_expression);
   double new_water_content = saturation_point - (saturation_point/hydraulic_slope) * log_expression;
   // Ensure that water content is not negative
   if (new_water_content < 0) {
      new_water_content = 0.0;
   }
   return (new_water_content * soil_thickness_mm);   // convert back to mm
}

void WaterBalance::ActualEvaporation(double potential_evaporation, double &actual_evaporation_layer_1, 
      double &actual_evaporation_layer_2)
{
   // Formula 5.24 on page 115
   // Calculates reduction of evaporation based on water content, constants from experiments
   double pow_expression = pow((3.6073 * water_amount_1 / saturated_hydraulic), -9.3172);
   double reduction_factor = 1.0 / (1.0 + pow_expression);
   // Formula 5.25 on page on page 115
   actual_evaporation_layer_1 = potential_evaporation * reduction_factor * 0.26;  // 26% from top layer
   actual_evaporation_layer_2 = potential_evaporation * reduction_factor * 0.74;  // rest from root zone
}

void WaterBalance::ActualTranspiration(double photosynthesis_efficiency, double potential_transpiration, 
      double &actual_transpiration_layer_1, double &actual_transpiration_layer_2)
{
   // volumetric water content critical point occurs when reduction factor = 1
   // Formula 5.29 on page 117
   double critical_point = wilting_point + 
                   photosynthesis_efficiency * (saturation_point - wilting_point);
   double reduction_factor = (water_amount_2 - wilting_point) /
                       (critical_point - wilting_point);
   // Reduction factor can't exceed 1.0
   if (reduction_factor > 1.0) {
      reduction_factor = 1.0;
   }
   actual_transpiration_layer_1 = 0.0;   // no active roots in top layer, so no transpiration only on second layer
   actual_transpiration_layer_2 = potential_transpiration * reduction_factor;   // all roots in root zone
}

double WaterBalance::VolumetricWaterContent(double soil_thickness, double volumetric_water_content, 
      double actual_evaporation, double actual_transpiration,
      double percolation_from_above, double &percolation_to_below)
{
   double soil_thickness_mm = 1000.0 * soil_thickness;     // for conversions
   double water_content_before = volumetric_water_content * soil_thickness_mm;    // want unit in mm
   water_content_before = WaterContentBeforeRedistribution(soil_thickness, water_content_before, percolation_from_above);
   double water_content_after = WaterContentAfterRedistribution(soil_thickness, water_content_before);
   water_content_after = water_content_after - actual_evaporation - actual_transpiration;
   if (water_content_after < 0.0) {
      water_content_after = 0.0;
   }
   percolation_to_below = water_content_before - actual_evaporation - actual_transpiration - water_content_after;  
   if (percolation_to_below < 0.0) {
      percolation_to_below = 0.0;
   }
   return (water_content_after / soil_thickness_mm);   // unit m3 m-3
}

void WaterBalance::DailyWaterContent()
{
   double potential_evaporation = dETs * 1000.0 / (2454000.0 * 998.0);
   double potential_transpiration = dETc * 1000.0 / (2454000.0 * 998.0);
   double actual_evaporation_layer_1 = 0.0, actual_evaporation_layer_2 = 0.0;
   ActualEvaporation(potential_evaporation, actual_evaporation_layer_1, actual_evaporation_layer_2);
   double actual_transpiration_layer_1 = 0.0, actual_transpiration_layer_2 = 0.0;
   ActualTranspiration(0.5, potential_transpiration, actual_transpiration_layer_1, actual_transpiration_layer_2);  // assume a C3 plant
   double dPerc01 = 0.0;
   water_amount_1 = VolumetricWaterContent(depth_1, water_amount_1, actual_evaporation_layer_1, actual_transpiration_layer_1,
                      rainfall, dPerc01);
   water_amount_2 = VolumetricWaterContent(depth_2, water_amount_2, actual_evaporation_layer_2, actual_transpiration_layer_2,
                      dPerc01, dPerc01);
}

// Fraction of growth reduced due to limited water. potT is potential
//   transpiration (mm day-1).
double WaterBalance::GrowthReduction(double potential_transpiration)
{
   double actual_transpiration_layer_1 = 0.0, actual_transpiration_layer_2 = 0.0;
   ActualTranspiration(0.5, potential_transpiration, actual_transpiration_layer_1, actual_transpiration_layer_2);  // C3 plant
   return (actual_transpiration_layer_2 / potential_transpiration);            // fraction (0 to 1)
}

}  // namespace environment

#endif
