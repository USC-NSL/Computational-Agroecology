#include "soil.h"

#include <limits>

namespace environment {

Soil::Soil(const Texture texture, const double pH, const double salinity,
           const double organic_matter, const double water_content_layer_1,
           const double water_content_layer_2)
    : texture_(texture),
      pH_(pH),
      salinity_(salinity),
      organic_matter_(organic_matter),
      water_content_(WaterBalance::DailyWaterContentReturn{
          water_content_layer_1, water_content_layer_2}) {}

void Soil::UpdateWaterContent(double rainfall,
                              double total_flux_density_sunlit_potential,
                              double total_flux_density_shaded_potential) {
  water_content_ = WaterBalance::DailyWaterContent(
      0 /* rainfall */, water_content_.water_amount_1,
      water_content_.water_amount_2, total_flux_density_sunlit_potential,
      total_flux_density_shaded_potential);
}

void Soil::AddWaterToSoil(double water_amount) {
  // TODO: Should all water just be added to the top layer?
  water_content_.water_amount_1 += water_amount;
}

bool operator==(const Soil &lhs, const Soil &rhs) {
  return (lhs.texture_ == rhs.texture_) && (lhs.pH_ == rhs.pH_) &&
         (lhs.salinity_ == rhs.salinity_) &&
         (lhs.organic_matter_ == rhs.organic_matter_) &&
         (lhs.water_content_.water_amount_1 ==
          rhs.water_content_.water_amount_1) &&
         (lhs.water_content_.water_amount_2 ==
          rhs.water_content_.water_amount_2);
}

}  // namespace environment