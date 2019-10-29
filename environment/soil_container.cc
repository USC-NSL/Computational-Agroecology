#include "soil_container.h"

namespace environment {

Soil *SoilContainer::GetSoil(const Coordinate &coordinate) {
  return &((soils_[size_t(coordinate.x)])[size_t(coordinate.y)]);
}

const Soil *SoilContainer::GetSoil(const Coordinate &coordinate) const {
  return &((soils_[size_t(coordinate.x)])[size_t(coordinate.y)]);
}

}  // namespace environment
