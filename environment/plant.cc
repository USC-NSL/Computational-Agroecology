#include "plant.h"

namespace environment {

int Plant::Harvest() {
  int ret = produce_;
  produce_ = 0;
  return ret;
}

void Plant::UpdateWaterContent(double rainfall,
                               double total_flux_density_sunlit_potential,
                               double total_flux_density_shaded_potential) {
  water_content_ = WaterBalance::DailyWaterContent(
      0 /* rainfall */, water_content_.water_amount_1,
      water_content_.water_amount_2, total_flux_density_sunlit_potential,
      total_flux_density_shaded_potential);
}

}  // namespace environment