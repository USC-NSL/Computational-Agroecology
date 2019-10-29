#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "agent/actions/action.h"
#include "config/terrain_raw_data.h"
#include "environment/coordinate.h"
#include "environment/plant_container.h"
#include "environment/soil_container.h"

namespace environment {

class Terrain {
 public:
  // Constructor
  // TODO: add more constructors to import different kinds of terrain
  // currently, this is just a dumb constructor which ignores lots of details
  Terrain(const config::TerrainRawData &terrain_raw_data);

  // Accessors
  int yield() const { return yield_; }
  size_t size() const { return size_; }
  size_t width() const { return size_; }
  size_t length() const { return size_; }

  // Returns the pointer to a plant on the specified coordinate.
  // This pointer is owned by this class. Thus, the caller should not take care
  // of the returning pointer.
  Plant *GetPlant(const Coordinate &coord) {
    return plant_container_.GetPlant(coord);
  }

  // Almost identical to the above accessor. Here returns a constant pointer
  // instead.
  const Plant *GetPlant(const Coordinate &coord) const {
    return plant_container_.GetPlant(coord);
  }

  // Returns the pointer to a cell of soil on the specified coordinate.
  // This pointer is owned by this class. Thus, the caller should not take care
  // of the returning pointer.
  Soil *GetSoil(const Coordinate &coord) {
    return soil_container_.GetSoil(coord);
  }

  // Almost identical to the above accessor. Here returns a constant pointer
  // instead.
  const Soil *GetSoil(const Coordinate &coord) const {
    return soil_container_.GetSoil(coord);
  }

  // Returns the full list of plants in this terrain
  std::vector<const Plant *> GetAllPlants() const {
    // Here we need to construct a new vector because the internal vector is
    // with type `std::vector<Plant *>` (the element is without the constant
    // specifier). Though the type conversion is safe, it is hard to be achieved
    // in C++.
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
