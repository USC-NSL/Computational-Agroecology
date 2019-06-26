#include "corn.h"

#include <string>

#include "utility.h"

namespace environment {

namespace plant_type {

const std::string kCornTypeName = "Corn";
const environment::Soil cornSoil(environment::Soil::CLAY, 6.0, 1.0, 2.0, 3.0,
                                 environment::Soil::HIGH,
                                 environment::Soil::HIGH);
const environment::Light cornLight(environment::Light::VERY_BRIGHT, 1.1);

Corn::Corn()
    : PlantType(kCornTypeName, "🌽", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0), MaxMinPair<int>({100, 10}),
                MaxMinPair<int>({100, 10}), MaxMinPair<int>(100, 10),
                MaxMinPair<int>(100, 10), cornSoil, cornSoil, cornLight,
                cornLight, MaxMinPair<int>(100, 10)) {}

environment::Plant Corn::GeneratePlantInstance() const {
  return Plant(kCornTypeName);
}

Corn __corn_type__;

}  // namespace plant_type

}  // namespace environment