#include "terrain.h"

#include <assert.h>
#include <vector>

#include "environment/meteorology.h"
#include "environment/plant.h"

namespace environment {

// `class Terrain`
Terrain::Terrain(const config::TerrainRawData &terrain_raw_data,
                 const Meteorology &meteorology)
    : size_(terrain_raw_data.size),
      yield_(terrain_raw_data.yield),
      soil_container_(terrain_raw_data.size),
      meteorology_(meteorology) {}

void Terrain::ExecuteAction(const agent::action::Action &action) {
  action.Execute(this);
}

std::ostream &operator<<(std::ostream &os, const Terrain &terrain) {
  // TODO: add output
  os << "this is plant." << std::endl;
  os << "this is soil." << std::endl;
  return os;
}

}  // namespace environment
