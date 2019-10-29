#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_GROW_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_GROW_H_

#include "environment/plant.h"

namespace environment {

class Grow {
 public:
  static void GrowPlant(const Plant &plant);
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_GROW_H_