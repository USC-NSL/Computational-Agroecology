// Copyright 2019
#ifndef COMPUTATIONAL_AGROECOLOGY_CONFIG_H
#define COMPUTATIONAL_AGROECOLOGY_CONFIG_H

#include <vector>
#include "climate.h"
#include "location.h"
#include "plant_type.h"
#include "../sim/terrain.h"

class Config {
 public:
  Config(Location loc, std::vector<PlantType> plants)
      : location_(loc), plants_(std::move(plants)) {
      *terrain_ = Terrain(100,100);
  }

  Location location() { return location_; }
  std::vector<PlantType> plants() { return plants_; }
  Terrain terrain() { return *terrain_; }

 private:
  Location location_;              // latitude and longitude of location
  std::vector<PlantType> plants_;  // all available plants in the field
  Terrain *terrain_;  // current terrain
};

#endif //COMPUTATIONAL_AGROECOLOGY_CONFIG_H