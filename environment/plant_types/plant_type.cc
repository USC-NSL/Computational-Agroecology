#include "plant_type.h"

#include <unordered_map>

namespace environment {

namespace plant_type {

PlantType::PlantType(const std::string& type_name,
                     const std::string& display_symbol, const bool cultivar,
                     const double base_temperature,
                     const MaxMinTemperature& optimal_temperature,
                     const MaxMinTemperature& absolute_temperature,
                     const MaxMinRainfall& optimal_annual_rainfall,
                     const MaxMinRainfall& absolute_annual_rainfall,
                     const MaxMinPair<int>& optimal_soil_depth,
                     const MaxMinPair<int>& absolute_soil_depth,
                     const MaxMinPair<int>& optimal_latitude,
                     const MaxMinPair<int>& absolute_latitude,
                     const Soil& optimal_soil_condition,
                     const Soil& absolute_soil_condition,
                     const Light& optimal_light_condition,
                     const Light& absolute_light_condition,
                     const MaxMinPair<int> crop_cycle)
    : type_name(type_name),
      display_symbol(display_symbol),
      cultivar(cultivar),
      base_temperature(base_temperature),
      optimal_temperature(optimal_temperature),
      absolute_temperature(absolute_temperature),
      optimal_annual_rainfall(optimal_annual_rainfall),
      absolute_annual_rainfall(absolute_annual_rainfall),
      optimal_soil_depth(optimal_soil_depth),
      absolute_soil_depth(absolute_soil_depth),
      optimal_latitude(optimal_latitude),
      absolute_latitude(absolute_latitude),
      crop_cycle(crop_cycle),
      optimal_soil_condition(optimal_soil_condition),
      absolute_soil_condition(absolute_soil_condition),
      optimal_light_condition(optimal_light_condition),
      absolute_light_condition(absolute_light_condition) {
  // register this type to the map
  Register();
}

environment::Plant PlantType::GeneratePlantInstance() const {
  return environment::Plant(type_name);
}

void PlantType::Register() { plant_type_to_plant[type_name] = this; }

// definition
std::map<std::string, const PlantType*> plant_type_to_plant;

}  // namespace plant_type

}  // namespace environment