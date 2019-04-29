// Copyright 2019
#ifndef AGROECOLOGY_COMMON_PLANT_H_
#define AGROECOLOGY_COMMON_PLANT_H_

#include <utility>
#include <string>


// Represents a single plant instance.
class PlantIndex {
 public:
  enum class Maturity { SEED, SEEDLING, JUVENILE, MATURE, OLD };

  static const int MAX_HEALTH = 10;
  static const int INITIAL_HEALTH = 10;
  static const int MIN_HEALTH = 0;

  // Initialize indexes of plant with given initial value
  PlantIndex()
      : health_(INITIAL_HEALTH),
        flowering_(false),
        accumulated_gdd_(0),
        maturity_(Maturity::SEED) {}
  bool CheckNeeds(double rainfall, double minTemp, double maxTemp);
  void Stage(int* thresholds);
  bool Transition(double rainfall, double minTemp, double maxTemp);
  int CalcGDD(double minTemp, double maxTemp);
  int health() const { return health_; }
  bool flowering() const { return flowering_; }
  int accumulated_gdd() const { return accumulated_gdd_; }
  Maturity maturity() const { return maturity_; }
  int base_temperature() { return base_temperature_; }

 private:
  int health_;  // [0,10] where 0 is dead and 10 is most healthy.

  bool flowering_;  // Is the plant currently flowering?

  // Accumulated Growing Degree Days, to be used by simulator.
  int accumulated_gdd_;

  // The plant's current maturity.
  Maturity maturity_;

  void IncrementMaturity();

  int base_temperature_;

  int gdd_thresholds_[5];
};

#endif  // AGROECOLOGY_COMMON_PLANT_H_
