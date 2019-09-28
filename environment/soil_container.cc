#include "soil_container.h"

namespace environment {

Soil *SoilContainer::GetSoil(const Coordinate &coordinate) {
  return &((soils_[size_t(coordinate.x)])[size_t(coordinate.y)]);
}

const Soil *SoilContainer::GetSoil(const Coordinate &coordinate) const {
  return &((soils_[size_t(coordinate.x)])[size_t(coordinate.y)]);
}

Soil *SoilContainer::GetSoil(const double x, const double y) {
  return &((soils_[size_t(x)])[size_t(y)]);
}

const Soil *SoilContainer::GetSoil(const double x, const double y) const {
  return &((soils_[size_t(x)])[size_t(y)]);
}

std::vector<Soil *> SoilContainer::GetMultipleSoil(const Coordinate &coordinate,
                                                   const double x_length,
                                                   const double y_length) {
  return GetMultipleSoil(coordinate.x, coordinate.y, x_length, y_length);
}

std::vector<Soil *> SoilContainer::GetMultipleSoil(const double x,
                                                   const double y,
                                                   const double x_length,
                                                   const double y_length) {
  std::vector<Soil *> soils;
  for (size_t i = (size_t)x; i < size_t(x + x_length); i++) {
    for (size_t j = (size_t)y; j < size_t(y + y_length); j++) {
      soils.push_back(&soils_[i][j]);
    }
  }
  return soils;
}

}  // namespace environment
