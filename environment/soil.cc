#include "soil.h"

namespace environment {

Soil::Soil(const Texture texture, const double pH, const double salinity,
           const double organic_matter, const double water_content,
           const SoilIndex fertility, const SoilIndex salinity_index)
    : texture(texture),
      fertility(fertility),
      pH(pH),
      salinity(salinity),
      organic_matter(organic_matter),
      water_content(water_content),
      salinity_index(salinity_index){}

bool operator==(const Soil& lhs, const Soil& rhs) {
  return (lhs.texture == rhs.texture) && (lhs.pH == rhs.pH) &&
         (lhs.salinity == rhs.salinity) &&
         (lhs.organic_matter == rhs.organic_matter) &&
         (lhs.water_content == rhs.water_content) &&
          (lhs.fertility == rhs.fertility) &&
          (lhs.salinity_index == rhs.salinity_index);
}

}  // namespace environment