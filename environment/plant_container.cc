#include "plant_container.h"

namespace environment {

bool PlantContainer::AddPlant(Plant *plant) {
  if (!CheckPosition(plant->position().To2DVector(), plant->trunk_size())) {
    return false;
  }
  plants_.push_back(plant);
  ContructPlantKDTree();
  return true;
}

bool PlantContainer::DelPlant(const Plant &plant) {
  size_t index = kdtree_->nearest_index(plant.position().To2DVector());
  if (IsSamePlant(*plants_[index], plant)) {
    plants_.erase(plants_.begin() + index);
    ContructPlantKDTree();
    return true;
  }
  return false;
}

Plant *PlantContainer::FindPlant(const Coordinate &coordinate) {
  point_t position = coordinate.To2DVector();
  size_t index = kdtree_->nearest_index(position);
  if (IsSamePlant(*plants_[index], position)) {
    return plants_[index];
  }
  return nullptr;
}

bool PlantContainer::DelPlant(const Coordinate &coordinate) {
  point_t position = coordinate.To2DVector();
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
  return plant.position() == position;
}

bool PlantContainer::IsSamePlant(const Plant &plant_a,
                                 const Plant &plant_b) const {
  return plant_a.position() == plant_b.position();
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
    points.push_back(plant->position().To2DVector());
  }
  kdtree_ = std::make_shared<KDTree>(points);
}

}  // namespace environment
