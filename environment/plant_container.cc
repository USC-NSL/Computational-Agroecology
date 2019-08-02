#include "plant_container.h"

namespace environment {

bool PlantContainer::AddPlant(Plant *plant) {
  if (!CheckPosition(plant->position(), plant->trunk_size()))
    return false;
  plants_.push_back(plant);
  ContructPlantKDTree();
  return true;
}

bool PlantContainer::DelPlant(const Plant &plant) {
  size_t index = kdtree_->nearest_index(plant.position());
  if (*plants_[index] == plant) {
    plants_.erase(plants_.begin() + index);
    ContructPlantKDTree();
    return true;
  }
  return false;
}

bool PlantContainer::DelPlant(const std::vector<double> position) {
  size_t index = kdtree_->nearest_index(position);
  if (SamePlant(plants_[index]->position(), position)) {
    // delete plants_[index];
    plants_.erase(plants_.begin() + index);
    ContructPlantKDTree();
    return true;
  }
  return false;
}  // namespace environment

bool PlantContainer::SamePlant(const point_t &lhs, const point_t &rhs) const {
  for (int i = 0; i < kDims; i++) {
    if (lhs[i] != rhs[i])
      return false;
  }
  return true;
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
    point_t position;
    for (int i = 0; i < kDims; i++) {
      position.push_back(plant->position()[i]);
    }
    points.push_back(position);
  }
  if (kdtree_)
    delete kdtree_;
  kdtree_ = new KDTree(points);
}

}  // namespace environment
