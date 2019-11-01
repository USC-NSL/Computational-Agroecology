#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_

#include <optional>

#include "environment/resource.h"
#include "environment/utility.h"
#include "environment/water_balance.h"

namespace environment {

// TODO: Refactor these nutrients into a map from ResourceType to amount, as in
// utility.h, and add to utility.h as needed.
// TODO: Merge Soil and SoilCondition.
// Ralph: The todos above are done.
class Soil {
 public:
  enum Texture { CLAY = 0, SILT, SAND };

  Soil(const Texture texture, const double pH, const double salinity,
       const double organic_matter, const double water_content_layer_1,
       const double water_content_layer_2);

  void UpdateWaterContent(double rainfall,
                          double total_flux_density_sunlit_potential,
                          double total_flux_density_shaded_potential);

  void AddWaterToSoil(double water_amount);

  const double pH() const { return pH_; }
  const double salinity() const { return salinity_; }
  const double organic_matter() const { return organic_matter_; }
  const Texture texture() const { return texture_; }
  const WaterBalance::DailyWaterContentReturn water_content() const {
    return water_content_;
  }

 private:
  Texture texture_;
  double pH_;
  double salinity_;
  double organic_matter_;

  Resources resources;

  WaterBalance::DailyWaterContentReturn water_content_;
};

bool operator==(const Soil &lhs, const Soil &rhs);

// `struct SoilRequirement` has been removed here.

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_
