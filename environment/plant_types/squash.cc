#include "squash.h"

#include "utility.h"

namespace environment {

namespace plant_type {

Squash::Squash()
    : PlantType(SQUASH, "squash", "-", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0)) {}

environment::Plant* Squash::GeneratePlantInstance() const {
  return new Plant(SQUASH);
}

Squash __squash_type__;

}  // namespace plant_type

}  // namespace environment