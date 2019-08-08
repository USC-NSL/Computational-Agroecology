#include "soil_container.h"

namespace environment {

Soil *SoilContainer::FindSoilByCoord(const double x, const double y) {
  return &((soils_[size_t(x)])[size_t(y)]);
}

Soil *SoilContainer::FindSoilByCoord(const Coordinate &coordinate) {
  return &((soils_[size_t(coordinate.x)])[size_t(coordinate.y)]);
}

std::vector<Soil *> SoilContainer::FindSoilByCoords(const double x, const double y,
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
