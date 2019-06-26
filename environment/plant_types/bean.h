#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_BEAN_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_BEAN_H_

#include "plant.h"
#include "plant_type.h"
#include "utility.h"
#include <string>

namespace environment {

namespace plant_type {

// TODO: define this
struct Bean : public PlantType {
  Bean();

  environment::Plant GeneratePlantInstance() const override;
};

}  // namespace plant_type

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_BEAN_H_