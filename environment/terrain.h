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
  Terrain(const config::TerrainRawData &terrain_raw_data,
          const Meteorology &meteorology);

  // Accessors
  int yield() const { return yield_; }
  size_t size() const { return size_; }
  size_t width() const { return size_; }
  size_t length() const { return size_; }
  SoilContainer &soil_container() { return soil_container_; }
  const SoilContainer &soil_container() const { return soil_container_; }
  PlantContainer &plant_container() { return plant_container_; }
  const PlantContainer &plant_container() const { return plant_container_; }

  // Modifiers
  void ExecuteAction(const agent::action::Action &action);

 private:
   // When a crop / plant is added, it acts on and has access only to the
  // "Terrain" object, and the newly created plant instance must have access to
  // meteorology. But meteorology is normally only held within the envrionment.
  // So we need to store a pointer to meteorology within the terrain
  const Meteorology &meteorology() const { return meteorology_; }
  
  friend std::ostream &operator<<(std::ostream &os, const Terrain &terrain);

  // befriend with a list of actions
  friend class agent::action::crop::Add;
  friend class agent::action::crop::Remove;
  friend class agent::action::crop::Harvest;
  friend class agent::action::crop::Water;

  PlantContainer plant_container_;
  SoilContainer soil_container_;
  const Meteorology meteorology_;

  int yield_;
  // TODO: Now we assume the terrain to be a square space.
  size_t size_;
};

std::ostream &operator<<(std::ostream &os, const Terrain &terrain);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
