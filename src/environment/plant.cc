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
      gdd_thresholds(),
      produce(0) {}

bool operator==(const Plant& lhs, const Plant& rhs) {
  return (lhs.type_name == rhs.type_name) && (lhs.health == rhs.health) &&
         (lhs.flowering == rhs.flowering) &&
         (lhs.accumulated_gdd == rhs.accumulated_gdd) &&
         (lhs.maturity == rhs.maturity) &&
         (lhs.base_temperature == rhs.base_temperature) &&
         (lhs.gdd_thresholds == rhs.gdd_thresholds);
}

}  // namespace environment