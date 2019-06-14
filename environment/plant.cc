#include "plant.h"

namespace environment {

const int kInitialHealth = 10;

Plant::Plant(const std::string& type_name)
    : type_name(type_name),
      health(kInitialHealth),
      flowering(false),
      accumulated_gdd(0),
      maturity(SEED),
      base_temperature(0.0),
      gdd_thresholds() {}

}  // namespace environment