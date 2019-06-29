#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_SQUASH_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_SQUASH_H_

#include <string>

#include "plant.h"
#include "plant_type.h"
#include "utility.h"

namespace environment {

namespace plant_type {

// TODO: define this
struct Squash : public PlantType {
  Squash();

  environment::Plant GeneratePlantInstance() const override;
};

}  // namespace plant_type

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_SQUASH_H_