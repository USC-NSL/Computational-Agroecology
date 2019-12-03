#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_

#include <vector>

#include "environment/coordinate.h"
#include "environment/soil.h"

namespace environment {

class SoilContainer : private std::vector<std::vector<Soil>> {
 public:
  // TODO: implement the constructor by physical unit
  // Temporary constructor
  // Currently constructs a square grid with size of `size`. In each grid the
  // soil instances are dumb instances.
  SoilContainer(const size_t size);

  // Retrieves a soil instance by giving its position (`struct Coordinate`).
  // For example, (*this)[Coordinate(1, 2)] fetches the soil on coordinate (1, 2).
  Soil &operator[](const Coordinate &coordinate);
  const Soil &operator[](const Coordinate &coordinate) const;

  // Retrieves a soil instance by giving its position (`struct Coordinate`)
  Soil &GetSoil(const Coordinate &coordinate);
  const Soil &GetSoil(const Coordinate &coordinate) const;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_