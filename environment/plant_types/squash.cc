#include "squash.h"

#include <string>

#include "utility.h"

namespace environment {

namespace plant_type {

const std::string kSquashTypeName = "Squash";
const environment::Soil squashSoil(environment::Soil::CLAY, 6.0, 1.0, 2.0, 3.0, environment::Soil::HIGH, environment::Soil::HIGH);
const environment::Light squashLight(environment::Light::VERY_BRIGHT, 1.1);

Squash::Squash()
    : PlantType(kSquashTypeName, "-", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0), MaxMinPair<int>({100, 10}), MaxMinPair<int>({100, 10}),
                MaxMinPair<int>(100, 10), MaxMinPair<int>(100, 10),
                squashSoil, squashSoil, squashLight, squashLight, MaxMinPair<int>(100, 10)) {}

environment::Plant Squash::GeneratePlantInstance() const {
  return Plant(kSquashTypeName);
}

Squash __squash_type__;

}  // namespace plant_type

}  // namespace environment