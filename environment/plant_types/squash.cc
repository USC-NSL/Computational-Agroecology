#include "squash.h"

#include <string>

#include "utility.h"

namespace environment {

namespace plant_type {

const std::string kSquashTypeName = "Squash";

Squash::Squash()
    : PlantType(kSquashTypeName, "-", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0)) {}

environment::Plant* Squash::GeneratePlantInstance() const {
  return new Plant(kSquashTypeName);
}

Squash __squash_type__;

}  // namespace plant_type

}  // namespace environment