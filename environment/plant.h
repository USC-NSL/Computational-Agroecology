#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_

#include <string>
#include <list>
#include <set>
#include "produce.h"
#include "plant_types/plant_type.h"

namespace environment {

// Represents a single plant instance.
// TODO: consider to define this as a `struct` instead of `class`
class Plant {
 public:
  // TODO: define some other constructors
  Plant(const std::string& type_name);

  enum Maturity { SEED = 0, SEEDLING, JUVENILE, MATURE, OLD };

  void growProduce() { this->produce_.insert(new Produce); }

  void IncrementProduceMaturity(Produce* temp);

  void updateProduceWeight(Produce* temp, double value) { temp->weight_ = value; }

  void increaseProduceWeight(Produce* temp, double additional) { temp->weight_ += additional; }

  void harvestReadyProduce();
  // TODO: add other member functions to complete this model

  inline const std::string& type_name() { return type_name_; }

  std::set<Produce*> getProduce_() { return produce_; }

 private:
  void IncrementMaturity();

  const std::string type_name_;

  // [0,10] where 0 is dead and 10 is most healthy.
  int health_;

  // Is the plant currently flowering?
  bool flowering_;

  // Accumulated Growing Degree Days, to be used by simulator.
  int accumulated_gdd_;

  // The plant's current maturity.
  Maturity maturity_;

  int base_temperature_;

  const std::vector<int> gdd_thresholds_;

  // list for the produce on a plant
  std::set<Produce*> produce_;

  // list for the produce harvested from a plant
  std::set<Produce*> produceHarvested;

  // total weight of produce made from a plant (more statistically important)
  double totalProduceWeight;

};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_