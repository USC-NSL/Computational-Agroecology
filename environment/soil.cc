#include "soil.h"

namespace environment {

Soil::Soil(const Texture texture, const double pH, const double salinity,
           const double organic_matter, const double water_content)
    : texture(texture),
      pH(pH),
      salinity(salinity),
      organic_matter(organic_matter),
      water_content(water_content) {}

bool operator==(const Soil& lhs, const Soil& rhs) {
  bool ret = true;
  ret &= (lhs.texture == rhs.texture);
  ret &= (lhs.pH == rhs.pH);
  ret &= (lhs.salinity == rhs.salinity);
  ret &= (lhs.organic_matter == rhs.organic_matter);
  ret &= (lhs.water_content == rhs.water_content);
  return ret;
}

}  // namespace environment