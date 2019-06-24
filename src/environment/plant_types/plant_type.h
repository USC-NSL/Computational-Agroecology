#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_

#include <map>
#include <string>
#include <vector>

#include "util/max_min_pair.h"

namespace environment {

// forward declaration to solve a cycle dependency
class Plant;

namespace plant_type {

struct PlantType {
  PlantType(const std::string& type_name, const std::string& display_symbol,
            const bool cultivar, const double base_temperature,
            const MaxMinTemperature& optimal_temperature,
            const MaxMinTemperature& absolute_temperature,
            const MaxMinRainfall& optimal_annual_rainfall,
            const MaxMinRainfall& absolute_annual_rainfall);

  // register itself to the map
  void Register();
  virtual environment::Plant GeneratePlantInstance() const = 0;

  const std::string type_name;
  const std::string display_symbol;
  const bool cultivar;

  const double base_temperature;
  const MaxMinTemperature optimal_temperature;
  const MaxMinTemperature absolute_temperature;

  const MaxMinRainfall optimal_annual_rainfall;
  const MaxMinRainfall absolute_annual_rainfall;
};

// global map declaration
// This global map maps from a type name to its corresponding `PlantType` object
// pointer
extern std::map<std::string, const PlantType*> plant_type_to_plant;

}  // namespace plant_type

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_