#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_

#include <map>
#include <string>
#include <vector>

#include "utility.h"
#include "soil.h"
#include "light.h"

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

  PlantType(const std::string& type_name, const std::string& display_symbol,
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
            const int min_crop_cycle);

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

  // Indicate how deep can plant's root go
  const int max_optimal_soil_depth;
  const int min_optimal_soil_depth;
  const int max_absolute_soil_depth;
  const int min_absolute_soil_depth;

  // Indicate the latitude that's suitable for plant growth
  const int max_optimal_latitude;
  const int min_optimal_latitude;
  const int max_absolute_latitude;
  const int min_absolute_latitude;

  // Indicate the soil fertility/salinity/Ph condition for the plant growth
  const Soil& optimal_soil_condition;
  const Soil& absolute_soil_condition;

  // Indicate the require Light condition require for the plant growth
  const Light& optimal_light_condition;
  const Light& absolute_light_condition;

  // The growth cycle for plant
  const int max_crop_cycle;
  const int min_crop_cycle;

};

// global map declaration
// This global map maps from a type name to its corresponding `PlantType` object
// pointer
extern std::map<std::string, const PlantType*> plant_type_to_plant;

}  // namespace plant_type

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_