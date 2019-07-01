#include "corn.h"

namespace environment {

namespace plant_type {

const std::string kCornTypeName = "Corn";

// Ref: http://ecocrop.fao.org/ecocrop/srv/en/dataSheet?id=2175
Corn::Corn()
    : PlantType(
          kCornTypeName,  // type_name
          "🌽",            // display_symbol
          true,           // cultivar
          environment::plant_type::PlantType::
              kSoilDepthRangeMedium,  // optimal_soil_depth
          environment::plant_type::PlantType::
              kSoilDepthRangeShallow,  // absolute_soil_depth
          // TODO: what is the base temperature of this
          std::nullopt,               // base_temperature
          MinMaxTemperature(18, 33),  // optimal_temperature
          MinMaxTemperature(10, 47),  // absolute_temperature
          MinMaxRainfall(600, 1200),  // optimal_annual_rainfall
          MinMaxRainfall(400, 1800),  // absolute_annual_rainfall
          environment::SoilCondition(
              environment::SoilCondition::SoilFertility::HIGH,  // fertility
              environment::SoilCondition::kSoilSalinityLow,     // salinity
              MinMaxPair<double>(5.0, 7.0),                     // pH
              std::nullopt                                      // drainage
              ),  // optimal_soil_condition
          environment::SoilCondition(
              environment::SoilCondition::SoilFertility::LOW,   // fertility
              environment::SoilCondition::kSoilSalinityMedium,  // salinity
              MinMaxPair<double>(4.5, 8.5),                     // pH
              std::nullopt                                      // drainage
              ),                       // absolute_soil_condition
          std::nullopt,                // optimal_latitude
          MinMaxPair<double>(40, 48),  // absolute_latitude
          environment::Light(
              MinMaxPair<environment::Light::LightIntensity>(
                  environment::Light::kLightIntensityVeryBright,
                  environment::Light::
                      kLightIntensityVeryBright),  // light_intensity
              std::nullopt                         // angle
              ),                                   // optimal_light_condition
          environment::Light(
              MinMaxPair<environment::Light::LightIntensity>(
                  environment::Light::kLightIntensityClearSkies,
                  environment::Light::
                      kLightIntensityVeryBright),  // light_intensity
              std::nullopt                         // angle
              ),                                   // absolute_light_condition
          std::nullopt,                            // climate_zone
          std::nullopt,                            // photo_period
          MinMaxPair<uint32_t>(65, 365)            // crop_cycle
      ) {}

environment::Plant Corn::GeneratePlantInstance() const {
  return Plant(kCornTypeName);
}

Corn __corn_type__;

}  // namespace plant_type

}  // namespace environment