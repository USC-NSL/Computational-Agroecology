#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_RADIATION_INFO_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_RADIATION_INFO_H_

#include <functional>
#include <utility>

namespace environment {

class PlantRadiationInfo {
 public:
  PlantRadiationInfo(
      const double lai, const double t_sr, const double t_ss, const double t_h,
      const std::function<double(const double)> &calculate_solar_elevation,
      const std::function<std::tuple<double, double, double>(const double)>
          &calculate_hourly_radiance);

  // Update all information by specifying the sunrise solar hour, the sunset
  // solar hour, and the current solar hour
  void UpdateDaily(const double t_sr, const double t_ss, const double t_h);

  // Only update information about the current solar hour
  void UpdateSolarHour(const double t_h);

 private:
  // reference labmda functions
  // They are used to call some functions related to and implemented in `class
  // SunInfo`.
  const std::function<double(const double)> &calculate_solar_elevation_;
  const std::function<std::tuple<double, double, double>(const double)>
      &calculate_hourly_radiance_;

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

  // Given a solar hour, calculate the extinction coefficient for direct
  // fluexes.
  double CalculateKDrBySolarHour(const double t_h) const;
  // Given solar elevation (in radians), calculate the extinction coefficient
  // for direct fluexes.
  static double CalculateKDr(const double solar_elevation);

  // Calculate canopy extinction coefficient for diffuse solar irradiance (k_df)
  double CalculateKDf() const;

  // Given a solar hour, calculate the interception of direct and diffuse solar
  // radiation. Notice: This should only be called by
  // `CalculateInterceptDailyRadiance()`.
  std::pair<double, double> CalculateInterceptHourlyRadiance(
      const double t_h) const;
  // Given direct and diffuse radiation, k_dr, and k_df, calculate the
  // interception of direct and diffuse solar radiation.
  std::pair<double, double> CalculateInterceptHourlyRadiance(
      const double I_dr, const double k_dr, const double I_df,
      const double k_df) const;

  // Given the solar time of sunrise and sunset, calculate the daily direct and
  // diffuse solar radiation.
  std::pair<double, double> CalculateInterceptDailyRadiance(
      const double t_sr, const double t_ss) const;

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