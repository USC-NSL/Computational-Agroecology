#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_SET_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_SET_H_

#include <unordered_map>
#include <utility>

#include "plant_type.h"

namespace environment {

namespace plant_type {

enum PlantTypePriority { MUST_HAVE = 0, MUST_NOT_HAVE };

using PlantTypeSet = std::unordered_map<PlantType, PlantTypePriority>;

}  // namespace plant_type

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_TYPES_PLANT_TYPE_SET_H_