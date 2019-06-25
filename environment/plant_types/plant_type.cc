#include "plant_type.h"

#include <unordered_map>

namespace environment {

namespace plant_type {
Soil dumb_soil(Soil::CLAY, 7.0, 0.0, 0.0, 0.0, Soil::HIGH, Soil::HIGH);
Light dumb_light(Light::VERY_BRIGHT, 1.1);

PlantType::PlantType(const std::string& type_name,
                     const std::string& display_symbol, const bool cultivar,
                     const double base_temperature,
                     const MaxMinTemperature& optimal_temperature,
                     const MaxMinTemperature& absolute_temperature,
                     const MaxMinRainfall& optimal_annual_rainfall,
                     const MaxMinRainfall& absolute_annual_rainfall)
    : type_name(type_name),
      display_symbol(display_symbol),
      cultivar(cultivar),
      base_temperature(base_temperature),
      optimal_temperature(optimal_temperature),
      absolute_temperature(absolute_temperature),
      optimal_annual_rainfall(optimal_annual_rainfall),
      absolute_annual_rainfall(absolute_annual_rainfall),
      max_optimal_soil_depth(0),
      min_optimal_soil_depth(0),
      max_absolute_soil_depth(0),
      min_absolute_soil_depth(0),
      max_optimal_latitude(0),
      min_optimal_latitude(0),
      max_absolute_latitude(0),
      min_absolute_latitude(0),
      min_crop_cycle(0),
      max_crop_cycle(0),
      optimal_light_condition(dumb_light),
      absolute_light_condition(dumb_light),
      optimal_soil_condition(dumb_soil),
      absolute_soil_condition(dumb_soil){
  // register this type to the map
  Register();
}

PlantType::PlantType(const std::string& type_name, const std::string& display_symbol,
                     const bool cultivar, const double base_temperature,
                     const MaxMinTemperature& optimal_temperature,
                     const MaxMinTemperature& absolute_temperature,
                     const MaxMinRainfall& optimal_annual_rainfall,
                     const MaxMinRainfall& absolute_annual_rainfall,
                     const int max_optimal_soil_depth,
                     const int min_optimal_soil_depth,
                     const int max_absolute_soil_depth,
                     const int min_absolute_soil_depth,
                     const double max_optimal_latitude,
                     const double min_optimal_latitude,
                     const double max_absolute_latitude,
                     const double min_absolute_latitude,
                     const Soil& optimal_soil_condition,
                     const Soil& absolute_soil_condition,
                     const Light& optimal_light_condition,
                     const Light& absolute_light_condition,
                     const int max_crop_cycle,
                     const int min_crop_cycle)
    : type_name(type_name),
      display_symbol(display_symbol),
      cultivar(cultivar),
      base_temperature(base_temperature),
      optimal_temperature(optimal_temperature),
      absolute_temperature(absolute_temperature),
      optimal_annual_rainfall(optimal_annual_rainfall),
      absolute_annual_rainfall(absolute_annual_rainfall),
      max_optimal_soil_depth(max_optimal_soil_depth),
      min_optimal_soil_depth(min_optimal_soil_depth),
      max_absolute_soil_depth(max_absolute_soil_depth),
      min_absolute_soil_depth(min_absolute_soil_depth),
      max_optimal_latitude(max_optimal_latitude),
      min_optimal_latitude(min_optimal_latitude),
      max_absolute_latitude(max_absolute_latitude),
      min_absolute_latitude(min_absolute_latitude),
      min_crop_cycle(min_crop_cycle),
      max_crop_cycle(max_crop_cycle),
      optimal_soil_condition(optimal_soil_condition),
      absolute_soil_condition(absolute_soil_condition),
      optimal_light_condition(optimal_light_condition),
      absolute_light_condition(absolute_light_condition){
  // register this type to the map
  Register();
}

void PlantType::Register() { plant_type_to_plant[type_name] = this; }

// definition
std::map<std::string, const PlantType*> plant_type_to_plant;

}  // namespace plant_type

}  // namespace environment