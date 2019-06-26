#include "bean.h"

namespace environment {

namespace plant_type {

const std::string kBeanTypeName = "Bean";
const environment::Soil beanSoil(environment::Soil::CLAY, 6.0, 1.0, 2.0, 3.0,
                                 environment::Soil::HIGH,
                                 environment::Soil::HIGH);
const environment::Light beanLight(environment::Light::VERY_BRIGHT, 1.1);

Bean::Bean()
    : PlantType(kBeanTypeName, "o", true, 0.0, MaxMinTemperature(0, 0),
                MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                MaxMinRainfall(0, 0), MaxMinPair<int>(100, 10),
                MaxMinPair<int>(100, 10), MaxMinPair<int>(100, 10),
                MaxMinPair<int>(100, 10), beanSoil, beanSoil, beanLight,
                beanLight, MaxMinPair<int>(100, 10)) {}

environment::Plant Bean::GeneratePlantInstance() const {
  return Plant(kBeanTypeName);
}

Bean __bean_type__;

}  // namespace plant_type

}  // namespace environment