#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_

#include <optional>

#include "utility.h"

namespace environment {

struct Soil {
  enum Texture { CLAY = 0, SILT, SAND };

  Soil(const Texture texture, const double pH, const double salinity,
       const double organic_matter, const double water_content);

  Texture texture;
  double pH;
  double salinity;
  double organic_matter;

  struct Nutrients {
    double nitrogen;
    double phosphorus;
    double potassium;
    double calcium;
    double magnesium;
    double sulfur;
  };

  double water_content;
};

bool operator==(const Soil& lhs, const Soil& rhs);

// TODO: `double` should be replaced with some density units
using SoilSalinity = MinMaxPair<double>;

// TODO: may need to add soil texture here
struct SoilCondition {
  enum class SoilFertility { LOW = 0, MODERATE, HIGH };

  // Poorly: (saturated >50% of year)
  // Well: (dry spells)
  // Excessive: (dry/moderately dry)
  enum class SoilDrainage { POORLY = 0, WELL, EXCESSIVE };

  SoilCondition(const std::optional<SoilFertility>& fertility,
                const std::optional<SoilSalinity>& salinity,
                const std::optional<MinMaxPair<double>>& pH,
                const std::optional<SoilDrainage>& drainage);

  const std::optional<SoilFertility> fertility;

  // Low (<<4 dS/m)
  // Medium (4-10 dS/m)
  // High (>10 dS/m)
  static const SoilSalinity kSoilSalinityLow;
  static const SoilSalinity kSoilSalinityMedium;
  static const SoilSalinity kSoilSalinityHigh;

  // TODO: consider whether to use references
  // It seems to make sense to use references but std::optional does not support
  // this natively.
  const std::optional<SoilSalinity> salinity;
  const std::optional<MinMaxPair<double>> pH;
  const std::optional<SoilDrainage> drainage;
};

bool operator==(const SoilCondition& lhs, const SoilCondition& rhs);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_