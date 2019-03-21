#include "../location.h"

class Config {
 public:
  Location getLoc() { return loc; }
  std::vector<PlantType> getPlants() { return plants; }

 private:
  Location loc_;                   // latitude and longitude of location
  std::vector<PlantType> plants_;  // all available plants to use