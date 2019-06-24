#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_CORN_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_CORN_H_

#include "environment/plant.h"
#include "environment/plant_types/plant_type.h"

namespace environment {

namespace plant_type {

// TODO: define this
struct Corn : public PlantType {
  Corn();

  environment::Plant GeneratePlantInstance() const override;
};

}  // namespace plant_type

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_CORN_H_