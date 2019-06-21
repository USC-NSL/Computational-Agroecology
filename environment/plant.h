#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_

#include <string>

#include "plant_types/plant_type.h"

namespace environment {

// Represents a single plant instance.
struct Plant {
  // TODO: define some other constructors
  Plant(const std::string& type_name);

  enum Maturity { SEED = 0, SEEDLING, JUVENILE, MATURE, OLD };

  // TODO: may need to add other member functions to complete this model
  // probably those functions should not be put here
  // instead they should be put in the simulator

  // Remove `void IncrementMaturity()`;
  // I think it is better to put it in the simulator

  // This variable is not supposed to modify after initialization
  std::string type_name;

  // [0,10] where 0 is dead and 10 is most healthy.
  int health;

  // Is the plant currently flowering?
  bool flowering;

  // Accumulated Growing Degree Days, to be used by simulator.
  int accumulated_gdd;

  // The plant's current maturity.
  Maturity maturity;

  // The current total weight of ripen fruit/vegetable/special crop in the
  // single crop in kg
  // TODO: Allow smaller granularity of produce process to happen
  int produce;

  double base_temperature;

  std::vector<int> gdd_thresholds;
};

extern const int kInitialHealth;
bool operator==(const Plant& lhs, const Plant& rhs);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_