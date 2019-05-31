#include "corn.h"

#include "utility.h"

namespace environment {

namespace plant_type {

Corn::Corn()
    : PlantType(CORN, "corn", "🌽", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0)) {}

environment::Plant* Corn::GeneratePlantInstance() const {
  return new Plant(CORN);
}

Corn __corn_type__;

}  // namespace plant_type

}  // namespace environment