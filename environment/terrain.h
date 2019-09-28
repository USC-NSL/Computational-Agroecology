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
  int yield() const { return yield_; }
  size_t size() const { return size_; }
  size_t width() const { return size_; }
  size_t length() const { return size_; }

  Plant *GetPlant(const Coordinate &coord) {
    return plant_container_.GetPlant(coord);
  }

  const Plant *GetPlant(const Coordinate &coord) const {
    return plant_container_.GetPlant(coord);
  }

  Soil *GetSoil(const Coordinate &coord) {
    return soil_container_.GetSoil(coord);
  }

  const Soil *GetSoil(const Coordinate &coord) const {
    return soil_container_.GetSoil(coord);
  }

  std::vector<const Plant *> GetAllPlants() const {
    return std::vector<const Plant *>(plant_container_.plants().begin(),
                                      plant_container_.plants().end());
  }

  // Modifiers
  void ExecuteAction(const agent::action::Action &action);

 private:
  friend std::ostream &operator<<(std::ostream &os, const Terrain &terrain);

  // befriend with a list of actions
  friend class agent::action::crop::Add;
  friend class agent::action::crop::Remove;
  friend class agent::action::crop::Harvest;
  friend class agent::action::crop::Water;

  PlantContainer plant_container_;
  SoilContainer soil_container_;

  int yield_;
  // TODO: Now we assume the terrain to be a square space.
  size_t size_;
};

std::ostream &operator<<(std::ostream &os, const Terrain &terrain);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
