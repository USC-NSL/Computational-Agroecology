#include "plant_container.h"

#include "environment/plant_builder.h"

namespace environment {

PlantContainer::~PlantContainer() {
  for (const auto *p : plants_) {
    delete p;
  }
}

Plant *PlantContainer::AddPlant(const std::string &plant_name,
                                const Coordinate &coordinate) {
  Plant *new_plant = PlantBuilder::NewPlant(plant_name);
  if (new_plant == nullptr) {
    return nullptr;
  }

  new_plant->position_ = coordinate;
  if (!CheckPosition(coordinate.To2DVector(), new_plant->trunk_size())) {
    delete new_plant;
    return nullptr;
  }

  plants_.push_back(new_plant);
  ConstructPlantKDTree();
  return new_plant;
}

bool PlantContainer::DelPlant(const Plant &plant) {
  return DelPlant(plant.position());
}

bool PlantContainer::DelPlant(const Coordinate &coordinate) {
  point_t position = coordinate.To2DVector();
  size_t index = kdtree_->nearest_index(position);
  if (IsSameLocationIn2D(plants_[index]->position(), coordinate)) {
    delete plants_[index];
    plants_.erase(plants_.begin() + index);
    ConstructPlantKDTree();
    return true;
  }
  return false;
}

Plant *PlantContainer::GetPlant(const Coordinate &coordinate) {
  point_t position = coordinate.To2DVector();
  size_t index = kdtree_->nearest_index(position);
  if (index < plants_.size() &&
      IsSameLocationIn2D(plants_[index]->position(), coordinate)) {
    return plants_[index];
  }
  return nullptr;
}

const Plant *PlantContainer::GetPlant(const Coordinate &coordinate) const {
  return GetPlant(coordinate);
}

bool PlantContainer::CheckPosition(const Coordinate &position,
                                   const double size) {
  if (!kdtree_)
    return true;
  auto res = kdtree_->neighborhood(position.To2DVector(), size);
  return res.empty();
}

void PlantContainer::ConstructPlantKDTree() {
  pointVec points;
  for (const auto &plant : plants_) {
    points.push_back(plant->position().To2DVector());
  }
  kdtree_ = std::make_shared<KDTree>(points);
}

}  // namespace environment
