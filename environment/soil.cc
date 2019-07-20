#include "soil.h"

#include <limits>

namespace environment {

Soil::Soil(const Texture texture, const double pH, const double salinity,
           const double organic_matter, const double water_content)
    : texture(texture),
      pH(pH),
      salinity(salinity),
      organic_matter(organic_matter),
      water_content(water_content) {}

bool operator==(const Soil &lhs, const Soil &rhs) {
  return (lhs.texture == rhs.texture) && (lhs.pH == rhs.pH) &&
         (lhs.salinity == rhs.salinity) &&
         (lhs.organic_matter == rhs.organic_matter) &&
         (lhs.water_content == rhs.water_content);
}

}  // namespace environment