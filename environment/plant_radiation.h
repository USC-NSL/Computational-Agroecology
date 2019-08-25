#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_RADIATION_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_RADIATION_H_

#include <functional>
#include <utility>

#include "environment/meteorology.h"

namespace environment {

// Represents the amount radiance that a plant can absorb. It requires the
// leaf index area of a plant and current information about sun in the
// environment to make this class work. This class focuses on a single plant.
class PlantRadiation {
 public:
  PlantRadiation(const double leaf_index_area, const Meteorology &meteorology);

  // Update all information according to the provided `Meteorology`.
  void Update(const Meteorology &meteorology);

  // Update all information by specifying the sunrise solar hour, the day of the
  // year. Calling this function utilizes the private `meteorology_`.
  void Update(const int day_of_year, const double solar_hour);

  // TODO: add accessors for other classes to use

 private:
  friend class EnergyBalance;

  // Have a private `meteorology_` so that this class can work independently.
  // Though it may seem to weird to have another `class Meteorology` here, this
  // class needs this in some cases, such as calling
  // `CalculateInterceptDailyRadiance()`.
  Meteorology meteorology_;

  // Leaf area index: leaf area per unit ground area (unit-less)
  // This is denoted as L in the book.
  const double total_leaf_area_index_;
  // The total area of sunlit leaves in a unit ground area (unit-less)
  // This is denoted as L_sl in the book.
  double sunlit_leaf_area_index_;
  // The total area of shaded leaves in a unit ground area (unit-less)
  // This is denoted as L_sh in the book.
  double shaded_leaf_area_index_;

  // A constant that is only related by `total_leaf_area_index_`. We store this
  // because it can be shared by many member functions.
  // This is denoted as k_df in the book.
  const double kExtinctionCoefficientForDiffuse;

  // Solar radiation intercepted by the canopies
  struct InterceptRadiance {
    // Direct solar radiation intercepted by the canopies (W m^-2)
    double direct;
    // Diffuse solar radiation intercepted by the canopies (W m^-2)
    double diffuse;
  };
  // Hourly direct and diffuse solar radiation intercepted by the canopies
  // They are denoted as I_i_dr and I_i_df in the book.
  InterceptRadiance hourly_solar_radiation_;

  // Daily direct and diffuse solar radiation intercepted by the canopies
  // They are denoted as I_i_dr_d and I_i_df_d in the book.
  InterceptRadiance daily_solar_radiation_;

  // The total flux density absorbed by the sunlit leaves per unit leaf area
  // (W m^-2)
  // This is denoted as I_sl or Q_sl in the book.
  double total_flux_density_sunlit_;
  // The total flux density absorbed by the shaded leaves per unit leaf area
  // (W m^-2)
  // This is denoted as I_sh or Q_sh in the book.
  double total_flux_density_shaded_;

  // Only update information about the current solar hour according to the
  // provided `meteorology`.
  void UpdateSolarHour(const Meteorology &meteorology);
  // Only update information about the current solar hour (in hours). Calling
  // this utilizes the private `meteorology_`.
  void UpdateSolarHour(const double solar_hour);

  // Given solar elevation (in radians), calculate the extinction coefficient
  // for direct fluexes (k_dr).
  static double CalculateExtinctionCoefficientForDirect(
      const double solar_elevation);

  // Calculate canopy extinction coefficient for diffuse solar irradiance (k_df)
  double CalculateExtinctionCoefficientForDiffuse() const;

  // Given direct and diffuse radiation, extinction coefficient for direct
  // fluexes (k_dr), and extinction coefficient for diffuse fluexes (k_df),
  // calculate the interception of direct and diffuse solar radiation (W m^-2).
  InterceptRadiance CalculateInterceptHourlyRadiance(
      const double direct_radiation, const double extinction_coefficient_direct,
      const double diffuse_radiation,
      const double extinction_coefficient_diffuse) const;

  // Given the solar time of sunrise and sunset (in solar hour), calculate the
  // daily direct and diffuse solar radiation (W m^-2).
  InterceptRadiance CalculateInterceptDailyRadiance(
      const double solar_hour_sunrise, const double solar_hour_sunset);

  // Photosynthetically Active Radiation is usually abbreviated as PAR.
  struct AbsorbedPhotosyntheticallyActiveRadiation {
    // Photosynthetically active radiation absored by sunlit leaves
    double sunlit;
    // Photosynthetically active radiation absored by shaded leaves
    double shaded;
  };
  // Given direct, diffuse radiation (W m^-2), and extinction coefficient for
  // direct fluexes (k_dr), calculate the total flux density absorbed by sunlit
  // leaves and shaded leaves (W m^-2).
  AbsorbedPhotosyntheticallyActiveRadiation CalculateAbsorbedHourPAR(
      const double direct_radiation, const double diffuse_radiation,
      const double extinction_coefficient_direct) const;

  struct LeafIndexArea {
    // Sunlit leaf area index (the total area of sunlit leaves in a unit ground
    // area)
    double sunlit;
    // Shaded leaf area index (the total area of shaded leaves in a unit ground
    // area)
    double shaded;
  };
  // Given k_dr (a calculated constant), calculate the sunlit leaf area index
  // (sunlit leaf area per unit ground area) and shaded leaf area index (shaded
  // leaf area per unit ground area).
  LeafIndexArea CalculateLai(const double k_dr) const;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_RADIATION_H_