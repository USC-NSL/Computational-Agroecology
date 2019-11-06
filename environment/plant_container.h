#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_

#include <string>

#include "KDTree/KDTree.hpp"
#include "environment/coordinate.h"
#include "environment/plant.h"

namespace environment {

class PlantContainer {
 public:
  ~PlantContainer();

  Plant *AddPlant(const std::string &plant_name, const Coordinate &coordinate);
  bool DelPlant(const Plant &plant);
  bool DelPlant(const Coordinate &coordinate);

  Plant *GetPlant(const Coordinate &coordinate);
  const Plant *GetPlant(const Coordinate &coordinate) const;

  PlantContainer() : kdtree_(nullptr){};

  const std::vector<Plant *> &plants() const { return plants_; }

 private:
  bool CheckPosition(const Coordinate &position, const double size);
  void ConstructPlantKDTree();

  std::vector<Plant *> plants_;
  std::shared_ptr<KDTree> kdtree_;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
