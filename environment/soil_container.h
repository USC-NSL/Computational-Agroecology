#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_

#include "environment/soil.h"

#include <vector>

#include "environment/coordinate.h"

namespace environment {

class SoilContainer {
 public:
  // TODO: implement the constructor by physical unit
  SoilContainer(const size_t size)
      : soils_(size,
               std::vector<Soil>(size, Soil(Soil::CLAY, 7.0, 0.0, 0.0, 0.0))) {}
  Soil *FindSoilByCoord(const Coordinate &coordinate);
  Soil *FindSoilByCoord(const double x, const double y);
  std::vector<Soil *> FindSoilByCoords(const double x, const double y,
                                       const double x_length,
                                       const double y_length);

 private:
  std::vector<std::vector<Soil>> soils_;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_
