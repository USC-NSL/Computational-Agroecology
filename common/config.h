#include "../location.h"
#include "plant_type.h"

class Config {
 public:
  Config(Location loc, std::vector<PlantType> plants)
      : location_(loc), plants_(plants) {}

  Location location() { return location_; }
  std::vector<PlantType> plants() { return plants_; }

 private:
  Location location_;              // latitude and longitude of location
  std::vector<PlantType> plants_;  // all available plants to use
};