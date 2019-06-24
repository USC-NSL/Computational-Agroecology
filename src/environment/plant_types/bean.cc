#include "bean.h"

#include <string>

#include "utility.h"

namespace environment {

namespace plant_type {

const std::string kBeanTypeName = "Bean";

Bean::Bean()
    : PlantType(kBeanTypeName, "o", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0)) {}

environment::Plant Bean::GeneratePlantInstance() const {
  return Plant(kBeanTypeName);
}

Bean __bean_type__;

}  // namespace plant_type

}  // namespace environment