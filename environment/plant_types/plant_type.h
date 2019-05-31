#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "utility.h"

namespace environment {

// forward declaration to solve a cycle dependency
class Plant;

namespace plant_type {

enum type_id { BEAN = 0, CORN, SQUASH };

struct PlantType {
  PlantType(const type_id id, const std::string& name,
            const std::string& display_symbol, const bool cultivar,
            const double base_temperature,
            const MaxMinTemperature& optimal_temperature,
            const MaxMinTemperature& absolute_temperature,
            const MaxMinRainfall& optimal_annual_rainfall,
            const MaxMinRainfall& absolute_annual_rainfall);

  // register itself to the map
  void Register();
  virtual environment::Plant* GeneratePlantInstance() const = 0;

  const type_id id;
  const std::string name;
  const std::string display_symbol;
  const bool cultivar;

  const double base_temperature;
  const MaxMinTemperature optimal_temperature;
  const MaxMinTemperature absolute_temperature;

  const MaxMinRainfall optimal_annual_rainfall;
  const MaxMinRainfall absolute_annual_rainfall;
};

// global map declaration
extern std::unordered_map<int, const PlantType*> plant_type_to_plant;

}  // namespace plant_type

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_