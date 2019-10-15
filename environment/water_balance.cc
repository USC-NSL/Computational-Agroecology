#include "water_balance.h"

#include <cmath>

namespace environment {

double WaterBalance::WaterContentBeforeRedistribution(
    double soil_layer_thickness, double water_content,
    double percolation_from_above) {
  // calculates point at which the water can no longer hold more water
  double water_content_saturation = saturation_point * soil_layer_thickness *
                                    kMeterToMillimeter;  // for conversion to mm
  double percolation_to_below = 0.0;
  double total_water_store = water_content + percolation_from_above;
  // Percolate water below if the current layer is too saturated
  // From formula 5.8 on page 109
  if (total_water_store > water_content_saturation) {
    percolation_to_below = total_water_store - water_content_saturation;
  }
  return (water_content + percolation_from_above - percolation_to_below);
}

double WaterBalance::WaterContentAfterRedistribution(double soil_thickness,
                                                     double water_content) {
  double soil_thickness_mm =
      soil_thickness * kMeterToMillimeter;  // for conversion
  // From formula 5.3 on page 107
  // Volumetric WC = volume of water contained in a unit volume of soil
  double volumetric_water_content =
      water_content / soil_thickness_mm;  // want unit in m3 m-3
  // From formula 5.20 on page 114, used to calculate new water content in soil
  double exp_expression = exp((hydraulic_slope / saturation_point) *
                              (saturation_point - volumetric_water_content));
  double val_expression = (hydraulic_slope * saturated_hydraulic * interval) /
                          (soil_thickness * saturation_point);
  double log_expression = log(val_expression + exp_expression);
  double new_water_content =
      saturation_point - (saturation_point / hydraulic_slope) * log_expression;
  // Ensure that water content is not negative
  if (new_water_content < 0) {
    new_water_content = 0.0;
  }
  return (new_water_content * soil_thickness_mm);  // convert back to mm
}

// Only the water amount in layer 1 is relevant for evaporation, so water amount
// in layer 2 is ignored here
WaterBalance::ActualEvaporationReturn WaterBalance::ActualEvaporation(
    double potential_evaporation, double water_amount_layer_1) {
  // Formula 5.24 on page 115
  // Calculates reduction of evaporation based on water content, constants from
  // experiments
  double pow_expression =
      pow((3.6073 * water_amount_layer_1 / saturated_hydraulic), -9.3172);
  double reduction_factor = 1.0 / (1.0 + pow_expression);
  // Formula 5.25 on page on page 115
  return {
      potential_evaporation * reduction_factor * 0.26,  // layer 1
      potential_evaporation * reduction_factor * 0.74   // layer 2
  };
}

WaterBalance::ActualTranspirationReturn WaterBalance::ActualTranspiration(
    double photosynthesis_efficiency, double potential_transpiration,
    double water_amount_2) {
  // volumetric water content critical point occurs when reduction factor = 1
  // Formula 5.29 on page 117
  double critical_point =
      wilting_point +
      photosynthesis_efficiency * (saturation_point - wilting_point);
  double reduction_factor =
      (water_amount_2 - wilting_point) / (critical_point - wilting_point);
  // Reduction factor can't exceed 1.0
  if (reduction_factor > 1.0) {
    reduction_factor = 1.0;
  }
  return {0.0, potential_transpiration * reduction_factor};
}

WaterBalance::VolumetricWaterContentReturn WaterBalance::VolumetricWaterContent(
    double soil_thickness, double volumetric_water_content,
    double actual_evaporation, double actual_transpiration,
    double percolation_from_above) {
  double soil_thickness_mm =
      soil_thickness * kMeterToMillimeter;  // for conversions
  double water_content_before =
      volumetric_water_content * soil_thickness_mm;  // want unit in mm
  water_content_before = WaterContentBeforeRedistribution(
      soil_thickness, water_content_before, percolation_from_above);
  double water_content_after =
      WaterContentAfterRedistribution(soil_thickness, water_content_before);
  water_content_after =
      water_content_after - actual_evaporation - actual_transpiration;
  if (water_content_after < 0.0) {
    water_content_after = 0.0;
  }
  double percolation_to_below = water_content_before - actual_evaporation -
                                actual_transpiration - water_content_after;
  if (percolation_to_below < 0.0) {
    percolation_to_below = 0.0;
  }
  return {water_content_after / soil_thickness_mm, percolation_to_below};
}

// TODO: When calling this function,
//    Get dETs and dETc from "meteorology.cc" file, from
//    UpdateHourlyNetRadiation (or maybe some other function)
WaterBalance::DailyWaterContentReturn WaterBalance::DailyWaterContent(
    double rainfall, double water_amount_1, double water_amount_2, double dETs,
    double dETc) {
  double potential_evaporation = dETs * portential_factor;
  double potential_transpiration = dETc * portential_factor;
  ActualEvaporationReturn actual_evaporation =
      ActualEvaporation(potential_evaporation, water_amount_1);
  ActualTranspirationReturn actual_transpiration = ActualTranspiration(
      photosynthesis_efficiency_c3, potential_transpiration, water_amount_2);
  VolumetricWaterContentReturn water_content_layer_1 = VolumetricWaterContent(
      depth_1, water_amount_1, actual_evaporation.layer_1,
      actual_transpiration.layer_1, rainfall);
  VolumetricWaterContentReturn water_content_layer_2 = VolumetricWaterContent(
      depth_2, water_amount_2, actual_evaporation.layer_2,
      actual_transpiration.layer_2, water_content_layer_1.percolation_to_below);
  return {water_content_layer_1.volumetric_water_content,
          water_content_layer_2.volumetric_water_content};
}

// Fraction of growth reduced due to limited water. potT is potential
//   transpiration (mm day-1).
double WaterBalance::GrowthReduction(double potential_transpiration,
                                     double water_amount_2) {
  ActualTranspirationReturn actual_transpiration = ActualTranspiration(
      photosynthesis_efficiency_c3, potential_transpiration, water_amount_2);
  return (actual_transpiration.layer_2 /
          potential_transpiration);  // fraction (0 to 1)
}

}  // namespace environment
