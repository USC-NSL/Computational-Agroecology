#include "terrain.h"

#include <assert.h>
#include <vector>

#include "environment/plant.h"

namespace environment {

// `class Terrain`
Terrain::Terrain(const size_t size)
    : size_(size), yield_(0), soil_container_(size) {}

void Terrain::ExecuteAction(const agent::action::Action &action) {
  action.Execute(this);
}

std::ostream &operator<<(std::ostream &os, const Terrain &terrain) {
  //TODO: add output
  os << "this is plant."<<std::endl;
  os << "this is soil."<<std::endl;
  return os;
}

}  // namespace environment
