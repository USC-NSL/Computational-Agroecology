#ifndef AGROECOLOGY_COMMON_PLANT_TYPE_H_
#define AGROECOLOGY_COMMON_PLANT_TYPE_H_

#include <string>

// Represents a single type of plant.
// TODO: Specify whether this is a single species or single cultivar.
class PlantType {
 public:
 // TODO: Fix this class's format.
  std::string name;
  int maxOptimalTemperature;
  int minOptimalTemperature;
  int maxAbsoluteTemperature;
  int minAbsoluteTemperature;
  int maxAbsoluteRainfall;
  int minAbsoluteRainfall;
  int maxOptimalRainfall;
  int minOptimalRainfall;

  PlantType(std::string plantName, int MaxOT, int MinOT,
          int MaxAT, int MinAT) :
  name(plantName), maxOptimalTemperature(MaxOT),
  minOptimalTemperature(MinOT), maxAbsoluteTemperature(MaxAT),
  minAbsoluteTemperature(MinAT) { }

  PlantType(std::string plantName, int MaxOT, int MinOT, int MaxAT,
          int MinAT, int MaxAR, int MinAR, int MaxOR, int MinOR) :
          name(plantName), maxOptimalTemperature(MaxOT),
          minOptimalTemperature(MinOT), maxAbsoluteTemperature(MaxAT),
          minAbsoluteTemperature(MinAT), maxAbsoluteRainfall(MaxAR),
          minAbsoluteRainfall(MinAR), maxOptimalRainfall(MaxOR),
          minOptimalRainfall(MinOR) { }
};

#endif  // AGROECOLOGY_COMMON_PLANT_TYPE_H_
