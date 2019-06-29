#include "corn.h"

// TODO: This is just a temperary anonymous namespace. It should be removed some
// time in the future
namespace {

const environment::SoilCondition kNullSoilCondition(std::nullopt,  // fertility
                                                    std::nullopt,  // salinity
                                                    std::nullopt,  // pH
                                                    std::nullopt   // drainage
);

const environment::Light kNullLight(std::nullopt,  // light_intensity
                                    std::nullopt   // angle
);

}  // namespace

namespace environment {

namespace plant_type {

const std::string kCornTypeName = "Corn";

Corn::Corn()
    : PlantType(kCornTypeName,       // type_name
                "🌽",                 // display_symbol
                true,                // cultivar
                std::nullopt,        // optimal_soil_depth
                std::nullopt,        // absolute_soil_depth
                std::nullopt,        // base_temperature
                std::nullopt,        // optimal_temperature
                std::nullopt,        // absolute_temperature
                std::nullopt,        // optimal_annual_rainfall
                std::nullopt,        // absolute_annual_rainfall
                kNullSoilCondition,  // optimal_soil_condition
                kNullSoilCondition,  // absolute_soil_condition
                std::nullopt,        // optimal_latitude
                std::nullopt,        // absolute_latitude
                kNullLight,          // optimal_light_condition
                kNullLight,          // absolute_light_condition
                std::nullopt,        // climate_zone
                std::nullopt,        // photo_period
                std::nullopt         // crop_cycle
      ) {}

environment::Plant Corn::GeneratePlantInstance() const {
  return Plant(kCornTypeName);
}

Corn __corn_type__;

}  // namespace plant_type

}  // namespace environment