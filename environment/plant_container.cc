#include "plant_container.h"

namespace environment {

bool PlantContainer::AddPlant(Plant *plant) {
  if (!CheckPosition(plant->position(), plant->trunk_size())) {
    return false;
  }
  plants_.push_back(plant);
  ContructPlantKDTree();
  return true;
}

bool PlantContainer::DelPlant(const Plant &plant) {
  size_t index = kdtree_->nearest_index(plant.position());
  if (IsSamePlant(*plants_[index], plant)) {
    plants_.erase(plants_.begin() + index);
    ContructPlantKDTree();
    return true;
  }
  return false;
}

Plant *PlantContainer::FindPlant(const Coordinate &coordinate) {
  point_t position = coordinate.ToVector();
  size_t index = kdtree_->nearest_index(position);
  if (IsSamePlant(*plants_[index], position)) {
    return plants_[index];
  }
  return nullptr;
}

bool PlantContainer::DelPlant(const Coordinate &coordinate) {
  point_t position = coordinate.ToVector();
  size_t index = kdtree_->nearest_index(position);
  if (IsSamePlant(*plants_[index], position)) {
    plants_.erase(plants_.begin() + index);
    ContructPlantKDTree();
    return true;
  }
  return false;
}

bool PlantContainer::IsSamePlant(const Plant &plant,
                                 const point_t &position) const {
  for (int i = 0; i < kDimsOfKDTree; i++) {
    if (plant.position()[i] != position[i])
      return false;
  }
  return true;
}

bool PlantContainer::IsSamePlant(const Plant &plant_a,
                                 const Plant &plant_b) const {
  for (int i = 0; i < kDimsOfKDTree; i++) {
    if (plant_a.position()[i] != plant_b.position()[i])
      return false;
  }
  return true;
}

bool PlantContainer::CheckPosition(const point_t &position, const double size) {
  if (!kdtree_)
    return true;
  auto res = kdtree_->neighborhood(position, size);
  return res.empty();
}

void PlantContainer::ContructPlantKDTree() {
  pointVec points;
  for (const auto &plant : plants_) {
    point_t position(plant->position().begin(),
                     plant->position().begin() + kDimsOfKDTree);
    points.push_back(position);
  }
  kdtree_ = std::make_shared<KDTree>(points);
}

}  // namespace environment
