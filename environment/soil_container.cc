#include "soil_container.h"

namespace environment {

Soil* SoilContainer::FindSoilByCoord(double x, double y) {
  return &soils_[size_t(x)][size_t(y)];
}
std::vector<Soil *> SoilContainer::FindSoilByCoords(double x, double y,
                                                    double x_length,
                                                    double y_length) {
  std::vector<Soil *> soils;
  for (size_t i = (size_t)x; i < size_t(x + x_length); i++) {
    for (size_t j = (size_t)y; j < size_t(y + y_length); j++) {
      soils.push_back(&soils_[i][j]);
    }
  }
  return soils;
}

};  // namespace environment

}  // namespace environment