#include "corn.h"

#include <string>

#include "utility.h"

namespace environment {

namespace plant_type {

const std::string kCornTypeName = "Corn";

Corn::Corn()
    : PlantType(kCornTypeName, "🌽", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0)) {}

environment::Plant* Corn::GeneratePlantInstance() const {
  return new Plant(kCornTypeName);
}

Corn __corn_type__;

}  // namespace plant_type

}  // namespace environment