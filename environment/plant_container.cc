#include "plant_container.h"

namespace environment {

bool PlantContainer::AddPlant(Plant *new_plant) {
  if (!CheckPosition(new_plant->position(), new_plant->trunk_size()))
    return false;
  plants_.push_back(new_plant);
  ContructPlantKDTree();
}

bool PlantContainer::DelPlant(const int index) {
  delete plants_[index];
  plants_.erase(plants_.begin() + index);
  ContructPlantKDTree();
}

bool PlantContainer::CheckPosition(const point_t &position, const double size) {
  if (!kdtree_)
    return true;
  auto res = kdtree_->neighborhood(position, size);
  if (!res.empty())
    return false;
  else
    return true;
}

void PlantContainer::ContructPlantKDTree() {
  pointVec points;
  for (const auto &plant : plants_) {
    points.push_back(plant->position());
  }
  if (kdtree_)
    delete kdtree_;
  kdtree_ = new KDTree(points);
}

}  // namespace environment