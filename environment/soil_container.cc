#include "soil_container.h"

namespace environment {

SoilContainer::SoilContainer(const size_t size)
    : std::vector<std::vector<Soil>>(
          size,
          std::vector<Soil>(size, Soil(Soil::CLAY, 7.0, 0.0, 0.0, 0.0, 0.0))) {}

Soil &SoilContainer::operator[](const Coordinate &coordinate) {
  return std::vector<std::vector<Soil>>::at(coordinate.x).at(coordinate.y);
}

const Soil &SoilContainer::operator[](const Coordinate &coordinate) const {
  return std::vector<std::vector<Soil>>::at(coordinate.x).at(coordinate.y);
}

Soil &SoilContainer::GetSoil(const Coordinate &coordinate) {
  return operator[](coordinate);
}

const Soil &SoilContainer::GetSoil(const Coordinate &coordinate) const {
  return operator[](coordinate);
}

}  // namespace environment
