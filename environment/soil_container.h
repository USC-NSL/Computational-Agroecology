#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_

#include <vector>

#include "environment/coordinate.h"
#include "environment/soil.h"

namespace environment {

class SoilContainer {
 public:
  // TODO: implement the constructor by physical unit
  // Temporary constructor
  // Currently constructs a square grid with size of `size`. In each grid the
  // soil instances are dumb instances.
  SoilContainer(const size_t size)
      : soils_(size,
               std::vector<Soil>(size, Soil(Soil::CLAY, 7.0, 0.0, 0.0, 0.0))) {}

  // Retrieves a soil instance by giving its position (`struct Coordinate`)
  Soil *GetSoil(const Coordinate &coordinate);
  const Soil *GetSoil(const Coordinate &coordinate) const;

 private:
  std::vector<std::vector<Soil>> soils_;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_