#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "agent/actions/action.h"
#include "environment/coordinate.h"
#include "environment/plant_container.h"
#include "environment/soil_container.h"

namespace environment {

class Terrain {
 public:
  // Constructor
  // TODO: add more constructors to import different kinds of terrain
  // currently, this is just a dumb constructor which ignores lots of details
  Terrain(const size_t size);

  // Accessors
  inline const int yield() const { return yield_; }
  inline const size_t size() const { return size_; }

  // Modifiers
  void ExecuteAction(const agent::action::Action &action);

 private:
  PlantContainer plant_container_;
  SoilContainer soil_container_;
  friend std::ostream &operator<<(std::ostream &os, const Terrain &terrain);

  // befriend with a list of actions
  friend class agent::action::crop::Add;
  friend class agent::action::crop::Remove;
  friend class agent::action::crop::Harvest;
  friend class agent::action::crop::Water;

  int yield_;
  size_t size_;
};

std::ostream &operator<<(std::ostream &os, const Terrain &terrain);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
