#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTS_BEAN_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTS_BEAN_H_

#include <string>

#include "plant.h"
#include "utility.h"

namespace environment {

namespace plants {

class Bean : public Plant {
 public:
  Bean(const std::string &name) : Plant(name) {}

  // TODO: Fill in.
  virtual Resources GrowStep(const int64_t num_time_step,
                             const Resources &available) override {
    return Resources();
  }
};

}  // namespace plants

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTS_BEAN_H_
