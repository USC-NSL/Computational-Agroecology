#include "bean.h"

#include "utility.h"

namespace environment {

namespace plant_type {

Bean::Bean()
    : PlantType(BEAN, "bean", "o", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0)) {}

environment::Plant* Bean::GeneratePlantInstance() const {
  return new Plant(BEAN);
}

Bean __bean_type__;

}  // namespace plant_type

}  // namespace environment