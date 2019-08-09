#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_

#include "KDTree/KDTree.hpp"

#include "environment/coordinate.h"
#include "environment/plant.h"

namespace environment {

class PlantContainer {
 public:
  bool AddPlant(Plant *plant);
  bool DelPlant(const Plant &plant);
  bool DelPlant(const Coordinate &coordinate);
  Plant *FindPlant(const Coordinate &coordinate);
  PlantContainer() : kdtree_(nullptr){};

 private:
  std::vector<Plant *> plants_;
  std::shared_ptr<KDTree> kdtree_;
  bool IsSamePlant(const Plant &plant_a, const Plant &plant_b) const;
  bool IsSamePlant(const Plant &plant, const Coordinate &position) const;
  bool CheckPosition(const Coordinate &position, const double size);
  void ContructPlantKDTree();
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
