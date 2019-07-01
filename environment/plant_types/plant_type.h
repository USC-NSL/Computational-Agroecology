#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_

#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "climate.h"
#include "light.h"
#include "plant.h"
#include "soil.h"
#include "utility.h"

namespace environment {

// forward declaration to solve a cycle dependency
class Plant;

// TODO: `double` should be replaced with centimeters
using SoilDepthRange = MinMaxPair<double>;
// This `int` denotes the number of hours
using PhotoPeriod = MinMaxPair<uint32_t>;

namespace plant_type {
struct PlantType {
  PlantType(const std::string& type_name, const std::string& display_symbol,
            const bool cultivar,
            const std::optional<SoilDepthRange>& optimal_soil_depth,
            const std::optional<SoilDepthRange>& absolute_soil_depth,
            const std::optional<double>& base_temperature,
            const std::optional<MinMaxTemperature>& optimal_temperature,
            const std::optional<MinMaxTemperature>& absolute_temperature,
            const std::optional<MinMaxRainfall>& optimal_annual_rainfall,
            const std::optional<MinMaxRainfall>& absolute_annual_rainfall,
            const SoilCondition& optimal_soil_condition,
            const SoilCondition& absolute_soil_condition,
            const std::optional<MinMaxPair<double>>& optimal_latitude,
            const std::optional<MinMaxPair<double>>& absolute_latitude,
            const Light& optimal_light_condition,
            const Light& absolute_light_condition,
            const std::optional<std::set<environment::Climate::ZoneType>>&
                climate_zone,
            const std::optional<PhotoPeriod>& photo_period,
            const std::optional<MinMaxPair<uint32_t>>& crop_cycle);

  // register itself to the map
  void Register();
  virtual environment::Plant GeneratePlantInstance() const;

  const std::string type_name;
  const std::string display_symbol;
  const bool cultivar;

  // Soil Depth: Indicate how deep can plant's root go
  // Shallow (20-50 cm)
  // Medium (50-150 cm)
  // Deep (>>150 cm)
  static const SoilDepthRange kSoilDepthRangeShallow;
  static const SoilDepthRange kSoilDepthRangeMedium;
  static const SoilDepthRange kSoilDepthRangeDeep;

  // TODO: consider whether to use references
  // It seems to make sense to use references but std::optional does not support
  // this natively.
  const std::optional<SoilDepthRange> optimal_soil_depth;
  const std::optional<SoilDepthRange> absolute_soil_depth;

  // Temperature
  // TODO: this `double` should be replaced with `degC`
  const std::optional<double> base_temperature;
  const std::optional<MinMaxTemperature> optimal_temperature;
  const std::optional<MinMaxTemperature> absolute_temperature;

  // Rainfall
  const std::optional<MinMaxRainfall> optimal_annual_rainfall;
  const std::optional<MinMaxRainfall> absolute_annual_rainfall;

  // Soil condition: Indicate the soil fertility/salinity/Ph condition for the
  // plant growth
  // Edited: remove reference (SoilCondition&) here
  // I believe even some plant types might share a soil condition.
  // I estimate there will not be a lot of them, so I decided to embed this
  // object.
  const SoilCondition optimal_soil_condition;
  const SoilCondition absolute_soil_condition;

  // Latitude: Indicate the latitude that's suitable for plant growth
  // TODO: this `double` should be replaced with some length units
  const std::optional<MinMaxPair<double>> optimal_latitude;
  const std::optional<MinMaxPair<double>> absolute_latitude;

  // Light Intensity: Indicate the require Light condition require for the plant
  // growth
  // Edited: Same as SoilCondition.
  const Light optimal_light_condition;
  const Light absolute_light_condition;

  // Climate Zone
  const std::optional<std::set<environment::Climate::ZoneType>> climate_zone;

  // Photo Period
  const std::optional<PhotoPeriod> photo_period;

  // The growth cycle for plant
  // This `int` denotes the number of days
  const std::optional<MinMaxPair<uint32_t>> crop_cycle;
};

// global map declaration
// This global map maps from a type name to its corresponding `PlantType` object
// pointer
extern std::map<std::string, const PlantType*> plant_type_to_plant;

}  // namespace plant_type

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_H_