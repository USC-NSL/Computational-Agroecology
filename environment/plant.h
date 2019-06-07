#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_

#include "plant_types/plant_type.h"

namespace environment {

// Represents a single plant instance.
class Plant {
 public:
  // TODO: define some other constructors
  Plant(const plant_type::TypeId type);

  enum Maturity { SEED = 0, SEEDLING, JUVENILE, MATURE, OLD };

  const plant_type::TypeId type;

  // TODO: add other member functions to complete this model

 private:
  // [0,10] where 0 is dead and 10 is most healthy.
  int health_;

  // Is the plant currently flowering?
  bool flowering_;

  // Accumulated Growing Degree Days, to be used by simulator.
  int accumulated_gdd_;

  // The plant's current maturity.
  Maturity maturity_;
  void IncrementMaturity();

  int base_temperature_;

  const std::vector<int> gdd_thresholds_;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_