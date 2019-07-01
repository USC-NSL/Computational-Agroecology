#include "squash.h"

#include <limits>

namespace environment {

namespace plant_type {

const std::string kSquashTypeName = "Squash";

// Cucurbita maxima
// Ref: http://ecocrop.fao.org/ecocrop/srv/en/dataSheet?id=819
Squash::Squash()
    : PlantType(
          kSquashTypeName,  // type_name
          "-",              // display_symbol
          true,             // cultivar
          environment::plant_type::PlantType::
              kSoilDepthRangeDeep,  // optimal_soil_depth
          environment::plant_type::PlantType::
              kSoilDepthRangeShallow,  // absolute_soil_depth
          // TODO: what is the base temperature of this
          std::nullopt,               // base_temperature
          MinMaxTemperature(20, 30),  // optimal_temperature
          MinMaxTemperature(9, 38),   // absolute_temperature
          MinMaxRainfall(600, 1000),  // optimal_annual_rainfall
          MinMaxRainfall(450, 2700),  // absolute_annual_rainfall
          environment::SoilCondition(
              environment::SoilCondition::SoilFertility::HIGH,  // fertility
              environment::SoilCondition::kSoilSalinityLow,     // salinity
              MinMaxPair<double>(5.5, 7.5),                     // pH
              environment::SoilCondition::SoilDrainage::WELL    // drainage
              ),  // optimal_soil_condition
          environment::SoilCondition(
              environment::SoilCondition::SoilFertility::LOW,  // fertility
              environment::SoilCondition::kSoilSalinityLow,    // salinity
              MinMaxPair<double>(5.0, 8.5),                    // pH
              environment::SoilCondition::SoilDrainage::WELL   // drainage
              ),  // absolute_soil_condition
          // TODO: The data does not provide the max value for this
          MinMaxPair<double>(
              10, std::numeric_limits<double>::infinity()),  // optimal_latitude
          MinMaxPair<double>(20, 50),  // absolute_latitude
          environment::Light(
              MinMaxPair<environment::Light::LightIntensity>(
                  environment::Light::kLightIntensityVeryBright,
                  environment::Light::
                      kLightIntensityVeryBright),  // light_intensity
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
          PhotoPeriod(0, 24),            // photo_period
          MinMaxPair<uint32_t>(80, 140)  // crop_cycle
      ) {}

environment::Plant Squash::GeneratePlantInstance() const {
  return Plant(kSquashTypeName);
}

Squash __squash_type__;

}  // namespace plant_type

}  // namespace environment