#include "bean.h"

#include <limits>

namespace environment {

namespace plant_type {

const std::string kBeanTypeName = "Bean";

// Ref: http://ecocrop.fao.org/ecocrop/srv/en/dataSheet?id=2146
Bean::Bean()
    : PlantType(
          kBeanTypeName,  // type_name
          "o",            // display_symbol
          true,           // cultivar
          environment::plant_type::PlantType::
              kSoilDepthRangeMedium,  // optimal_soil_depth
          environment::plant_type::PlantType::
              kSoilDepthRangeShallow,  // absolute_soil_depth
          // TODO: what is the base temperature of this
          std::nullopt,               // base_temperature
          MinMaxTemperature(18, 28),  // optimal_temperature
          MinMaxTemperature(5, 32),   // absolute_temperature
          MinMaxRainfall(650, 1000),  // optimal_annual_rainfall
          MinMaxRainfall(250, 2600),  // absolute_annual_rainfall
          environment::SoilCondition(
              environment::SoilCondition::SoilFertility::HIGH,  // fertility
              environment::SoilCondition::kSoilSalinityLow,     // salinity
              MinMaxPair<double>(6.0, 7.0),                     // pH
              environment::SoilCondition::SoilDrainage::WELL    // drainage
              ),  // optimal_soil_condition
          environment::SoilCondition(
              environment::SoilCondition::SoilFertility::MODERATE,  // fertility
              environment::SoilCondition::kSoilSalinityLow,         // salinity
              MinMaxPair<double>(4.5, 8.6),                         // pH
              environment::SoilCondition::SoilDrainage::WELL        // drainage
              ),  // absolute_soil_condition
          // TODO: The data does not provide the max value for this
          MinMaxPair<double>(
              30, std::numeric_limits<double>::infinity()),  // optimal_latitude
          MinMaxPair<double>(50, 60),  // absolute_latitude
          environment::Light(
              MinMaxPair<environment::Light::LightIntensity>(
                  environment::Light::kLightIntensityVeryBright,
                  environment::Light::
                      kLightIntensityClearSkies),  // light_intensity
              std::nullopt                         // angle
              ),                                   // optimal_light_condition
          environment::Light(
              MinMaxPair<environment::Light::LightIntensity>(
                  environment::Light::kLightIntensityVeryBright,
                  environment::Light::
                      kLightIntensityCloudySkies),  // light_intensity
              std::nullopt                          // angle
              ),                                    // absolute_light_condition
          std::set<environment::Climate::ZoneType>(
              {environment::Climate::TropicalWetAndDry,
               environment::Climate::TropicalWet,
               environment::Climate::SteppeOrSemiArid,
               environment::Climate::SubtropicalHumid,
               environment::Climate::SubtropicalDrySummer,
               environment::Climate::SubtropicalDryWinter,
               environment::Climate::TemperateOceanic,
               environment::Climate::TemperateContinental,
               environment::Climate::TemperateWithHumidWinters,
               environment::Climate::TemperateWithDryWinters}),  // climate_zone
          // Short day + neutral day + long day
          PhotoPeriod(0, 24),             // photo_period
          MinMaxPair<uint32_t>(100, 150)  // crop_cycle
      ) {}

environment::Plant Bean::GeneratePlantInstance() const {
  return Plant(kBeanTypeName);
}

Bean __bean_type__;

}  // namespace plant_type

}  // namespace environment