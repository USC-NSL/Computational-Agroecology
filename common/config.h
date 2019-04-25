// Copyright 2019
#ifndef COMPUTATIONAL_AGROECOLOGY_CONFIG_H
#define COMPUTATIONAL_AGROECOLOGY_CONFIG_H

#include <vector>

#include "climate.h"
#include "location.h"
#include "plant_type.h"

class Config {
 public:
  Config(Location loc, std::vector<PlantType> plants)
      : location_(loc), plants_(std::move(plants)) {}

  Location location() { return location_; }
  std::vector<PlantType> plants() { return plants_; }

 private:
  Location location_;              // latitude and longitude of location
  std::vector<PlantType> plants_;  // all available plants to use
};

#endif //COMPUTATIONAL_AGROECOLOGY_CONFIG_H