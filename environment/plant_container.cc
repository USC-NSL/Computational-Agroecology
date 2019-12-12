#include "plant_container.h"

#include "environment/plant_builder.h"

namespace environment {

Plant *PlantContainer::operator[](const Coordinate &coordinate) {
  return GetPlant(coordinate);
}

const Plant *PlantContainer::operator[](const Coordinate &coordinate) const {
  return GetPlant(coordinate);
}

Plant *PlantContainer::AddPlant(const std::string &plant_name,
                                const Coordinate &coordinate,
                                const Meteorology &meteorology) {
  std::unique_ptr<Plant> new_plant(
      PlantBuilder::NewPlant(plant_name, meteorology));
  if (new_plant == nullptr) {
    return nullptr;
  }

  new_plant->position_ = coordinate;
  if (!CheckPosition(coordinate.To2DVector(), new_plant->trunk_size())) {
    return nullptr;
  }

  plants_.push_back(std::move(new_plant));
  ConstructPlantKDTree();
  return plants_.back().get();
}

bool PlantContainer::DelPlant(const Plant &plant) {
  return DelPlant(plant.position());
}

bool PlantContainer::DelPlant(const Coordinate &coordinate) {
  point_t position = coordinate.To2DVector();
  size_t index = kdtree_->nearest_index(position);
  if (IsSameLocationIn2D(plants_[index]->position(), coordinate)) {
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
    return plants_[index].get();
  }
  return nullptr;
}

const Plant *PlantContainer::GetPlant(const Coordinate &coordinate) const {
  return GetPlant(coordinate);
}

PlantContainer::iterator PlantContainer::begin() {
  return plants_.begin();
}
PlantContainer::const_iterator PlantContainer::begin() const {
  return plants_.begin();
}
PlantContainer::const_iterator PlantContainer::cbegin() const {
  return plants_.cbegin();
}
PlantContainer::iterator PlantContainer::end() {
  return plants_.end();
}
PlantContainer::const_iterator PlantContainer::end() const {
  return plants_.end();
}
PlantContainer::const_iterator PlantContainer::cend() const {
  return plants_.cend();
}

PlantContainer::reverse_iterator PlantContainer::rbegin() {
  return reverse_iterator(end());
}
PlantContainer::const_reverse_iterator PlantContainer::rbegin() const {
  return const_reverse_iterator(end());
}
PlantContainer::const_reverse_iterator PlantContainer::crbegin() const {
  return const_reverse_iterator(cend());
}
PlantContainer::reverse_iterator PlantContainer::rend() {
  return reverse_iterator(begin());
}
PlantContainer::const_reverse_iterator PlantContainer::rend() const {
  return const_reverse_iterator(begin());
}
PlantContainer::const_reverse_iterator PlantContainer::crend() const {
  return const_reverse_iterator(cbegin());
}

bool PlantContainer::CheckPosition(const Coordinate &position,
                                   const double size) {
  if (!kdtree_) {
    return true;
  }
  auto res = kdtree_->neighborhood(position.To2DVector(), size);
  return res.empty();
}

void PlantContainer::ConstructPlantKDTree() {
  pointVec points;
  for (const auto &plant : plants_) {
    points.push_back(plant->position().To2DVector());
  }
  kdtree_.reset(new KDTree(points));
}

}  // namespace environment
