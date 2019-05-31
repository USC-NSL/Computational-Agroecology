#include "plant_type.h"

#include <unordered_map>

namespace environment {

namespace plant_type {

PlantType::PlantType(const type_id id, const std::string& name,
                     const std::string& display_symbol, const bool cultivar,
                     const double base_temperature,
                     const MaxMinTemperature& optimal_temperature,
                     const MaxMinTemperature& absolute_temperature,
                     const MaxMinRainfall& optimal_annual_rainfall,
                     const MaxMinRainfall& absolute_annual_rainfall)
    : id(id),
      name(name),
      display_symbol(display_symbol),
      cultivar(cultivar),
      base_temperature(base_temperature),
      optimal_temperature(optimal_temperature),
      absolute_temperature(absolute_temperature),
      optimal_annual_rainfall(optimal_annual_rainfall),
      absolute_annual_rainfall(absolute_annual_rainfall) {
  // register this type to the map
  this->Register();
}

void PlantType::Register() { plant_type_to_plant[this->id] = this; }

// definition
std::unordered_map<int, const PlantType*> plant_type_to_plant;

}  // namespace plant_type

}  // namespace environment