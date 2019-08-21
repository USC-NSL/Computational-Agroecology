#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_RADIATION_INFO_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_RADIATION_INFO_H_

#include <functional>
#include <utility>

#include "environment/sun_info.h"

namespace environment {

class PlantRadiationInfo {
 public:
  PlantRadiationInfo(const double lai, const SunInfo &sun_info);

  // Update all information according to the provided `SunInfo`.
  void Update(const SunInfo &sun_info);

  // Update all information by specifying the sunrise solar hour, the day of the
  // year. Calling this function utilizes the private `sun_info_`.
  void Update(const int day_of_year, const double t_h);

 private:
  // Have a private sun info so that this class can work independently. Though
  // it may seem to weird to have another `SunInfo` here, this class needs this
  // in some cases, such as calling `CalculateInterceptDailyRadiance()`.
  SunInfo sun_info_;

  // Leaf area index
  // The information in this class is all about this leaf area index
  const double lai_;

  // A constant that is only related by `lai_`. We store this because it can
  // be shared by many member functions.
  const double k_df_;

  // Hourly direct solar radiation intercepted by the canopies
  double I_i_dr_;
  // Hourly diffuse solar radiation intercepted by the canopies
  double I_i_df_;

  // Daily direct solar radiation intercepted by the canopies
  double I_i_dr_d_;
  // Daily diffuse solar radiation intercepted by the canopies
  double I_i_df_d_;

  // The total flux density absorbed by the sunlit leaves
  double I_sunlit_;
  // The total flux density absorbed by the shaded leaves
  double I_shaded_;

  // Sunlit leaf area index (the total area of sunlit leaves in a unit ground
  // area)
  double lai_sunlit_;
  // Shaded leaf area index (the total area of shaded leaves in a unit ground
  // area)
  double lai_shaded_;

  // Only update information about the current solar hour according to the
  // provided `sun_info`.
  void UpdateSolarHour(const SunInfo &sun_info);
  // Only update information about the current solar hour. Calling this utilizes
  // the private `sun_info_`.
  void UpdateSolarHour(const double t_h);

  // Given solar elevation (in radians), calculate the extinction coefficient
  // for direct fluexes.
  static double CalculateKDr(const double solar_elevation);

  // Calculate canopy extinction coefficient for diffuse solar irradiance (k_df)
  double CalculateKDf() const;

  // Given direct and diffuse radiation, k_dr, and k_df, calculate the
  // interception of direct and diffuse solar radiation.
  std::pair<double, double> CalculateInterceptHourlyRadiance(
      const double I_dr, const double k_dr, const double I_df,
      const double k_df) const;

  // Given the solar time of sunrise and sunset, calculate the daily direct and
  // diffuse solar radiation.
  std::pair<double, double> CalculateInterceptDailyRadiance(const double t_sr,
                                                            const double t_ss);

  // Given direct, diffuse radiation, and k_dr, calculate the total flux density
  // absorbed by sunlit leaves and shaded leaves.
  std::pair<double, double> CalculateAbsorbedHourPAR(const double I_dr,
                                                     const double I_df,
                                                     const double k_dr) const;

  // Given k_dr, calculate the sunlit leaf area index and shaded leaf area
  // index.
  std::pair<double, double> CalculateLai(const double k_dr) const;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_RADIATION_INFO_H_