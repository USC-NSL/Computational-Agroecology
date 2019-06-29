#include "plant_type.h"

#include <limits>
#include <unordered_map>

namespace environment {

namespace plant_type {

PlantType::PlantType(
    const std::string& type_name, const std::string& display_symbol,
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
    const Light& optimal_light_condition, const Light& absolute_light_condition,
    const std::optional<std::set<environment::Climate::ZoneType>>& climate_zone,
    const std::optional<PhotoPeriod>& photo_period,
    const std::optional<MinMaxPair<uint32_t>>& crop_cycle)
    : type_name(type_name),
      display_symbol(display_symbol),
      cultivar(cultivar),
      optimal_soil_depth(optimal_soil_depth),
      absolute_soil_depth(absolute_soil_depth),
      base_temperature(base_temperature),
      optimal_temperature(optimal_temperature),
      absolute_temperature(absolute_temperature),
      optimal_annual_rainfall(optimal_annual_rainfall),
      absolute_annual_rainfall(absolute_annual_rainfall),
      optimal_soil_condition(optimal_soil_condition),
      absolute_soil_condition(absolute_soil_condition),
      optimal_latitude(optimal_latitude),
      absolute_latitude(absolute_latitude),
      optimal_light_condition(optimal_light_condition),
      absolute_light_condition(absolute_light_condition),
      climate_zone(climate_zone),
      photo_period(photo_period),
      crop_cycle(crop_cycle) {
  // register this type to the map
  Register();
}

void PlantType::Register() { plant_type_to_plant[type_name] = this; }

environment::Plant PlantType::GeneratePlantInstance() const {
  return environment::Plant(type_name);
}

const SoilDepthRange PlantType::kSoilDepthRangeShallow(20.0, 50.0);
const SoilDepthRange PlantType::kSoilDepthRangeMedium(50.0, 150.0);
const SoilDepthRange PlantType::kSoilDepthRangeDeep(
    150.0, std::numeric_limits<double>::infinity());

// definition
std::map<std::string, const PlantType*> plant_type_to_plant;

}  // namespace plant_type

}  // namespace environment