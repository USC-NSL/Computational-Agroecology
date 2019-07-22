#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_

#include <optional>

#include "resource.h"
#include "utility.h"

namespace environment {

// TODO: Refactor these nutrients into a map from ResourceType to amount, as in
// utility.h, and add to utility.h as needed.
// TODO: Merge Soil and SoilCondition.
// Ralph: The todos above are done.
struct Soil {
  enum Texture { CLAY = 0, SILT, SAND };

  Soil(const Texture texture, const double pH, const double salinity,
       const double organic_matter, const double water_content);

  Texture texture;
  double pH;
  double salinity;
  double organic_matter;

  Resources resources;

  double water_content;
};

bool operator==(const Soil &lhs, const Soil &rhs);

// `struct SoilRequirement` has been removed here.

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_
