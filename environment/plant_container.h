#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_

#include "KDTree/KDTree.hpp"
#include "environment/plant.h"
namespace environment {

class PlantContainer {
 public:
  bool AddPlant(Plant *plant);
  bool DelPlant(const Plant &plant);
  bool DelPlant(const std::vector<double> position);
  PlantContainer() : kdtree_(nullptr){};

 private:
  std::vector<Plant *> plants_;
  KDTree *kdtree_;
  bool SamePlant(const point_t &lhs, const point_t &rhs) const;
  bool CheckPosition(const point_t &position, const double size);
  void ContructPlantKDTree();
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
