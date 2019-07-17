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

SoilRequirement::SoilRequirement(const std::optional<SoilFertility> &fertility,
                                 const std::optional<SoilSalinity> &salinity,
                                 const std::optional<MinMaxPair<double>> &pH,
                                 const std::optional<SoilDrainage> &drainage)
    : fertility(fertility), salinity(salinity), pH(pH), drainage(drainage) {}

const SoilSalinity SoilRequirement::kSoilSalinityLow(0.0, 4.0);
const SoilSalinity SoilRequirement::kSoilSalinityMedium(4.0, 10.0);
const SoilSalinity SoilRequirement::kSoilSalinityHigh(
    10.0, std::numeric_limits<double>::infinity());

bool operator==(const SoilRequirement &lhs, const SoilRequirement &rhs) {
  return (lhs.fertility == rhs.fertility) && (lhs.salinity == rhs.salinity) &&
         (lhs.pH == rhs.pH) && (lhs.drainage == rhs.drainage);
}

}  // namespace environment