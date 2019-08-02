
#include "KDTree/KDTree.hpp"
#include "environment/plant.h"
namespace environment {

class PlantContainer {
 public:

  bool AddPlant(Plant *new_plant);
  bool DelPlant(const int index);
  bool CheckPosition(const point_t &position, const double size);
  void ContructPlantKDTree();

 private:
  std::vector<Plant *> plants_;
  KDTree *kdtree_;
};

}  // namespace environment