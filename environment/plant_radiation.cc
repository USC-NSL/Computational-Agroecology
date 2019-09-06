#include "plant_radiation.h"

#include <cmath>

namespace environment {

PlantRadiation::PlantRadiation(const double leaf_index_area,
                               const Meteorology &meteorology)
    : meteorology_(meteorology),
      total_leaf_area_index_(leaf_index_area),
      kExtinctionCoefficientForDiffuse(
          CalculateExtinctionCoefficientForDiffuse()) {
  // Update (Initialize) this class with the given `meteorology`.
  Update(meteorology_);
}

void PlantRadiation::Update(const Meteorology &meteorology) {
  // Update daily solar radiation.
  daily_solar_radiation_ = CalculateInterceptDailyRadiance(
      meteorology.solar_hour_sunrise_, meteorology.solar_hour_sunset_);
  // Update other member variables related to solar hour.
  UpdateSolarHour(meteorology);
}

void PlantRadiation::Update(const int day_of_year, const double solar_hour) {
  // Cautious: we are using the private update function.
  meteorology_.UpdateDayOfYear(day_of_year);
  // Cautious: we are using the private update function.
  meteorology_.UpdateLocalSolarHour(solar_hour);
  // This is using the private `meteorology_`.
  Update(meteorology_);
}

void PlantRadiation::UpdateSolarHour(const Meteorology &meteorology) {
  const double kExtinctionCoefficientForDirect =
      CalculateExtinctionCoefficientForDirect(meteorology.solar_elevation());

  hourly_solar_radiation_ = CalculateInterceptHourlyRadiance(
      meteorology.hourly_direct_irradiance(), kExtinctionCoefficientForDirect,
      meteorology.hourly_diffuse_irradiance(),
      kExtinctionCoefficientForDiffuse);

  AbsorbedPhotosyntheticallyActiveRadiation par = CalculateAbsorbedHourPAR(
      meteorology.hourly_direct_irradiance(),
      meteorology.hourly_diffuse_irradiance(), kExtinctionCoefficientForDirect);
  total_flux_density_sunlit_ = par.sunlit;
  total_flux_density_shaded_ = par.shaded;

  LeafIndexArea lai = CalculateLai(kExtinctionCoefficientForDirect);
  sunlit_leaf_area_index_ = lai.sunlit;
  shaded_leaf_area_index_ = lai.shaded;
}

void PlantRadiation::UpdateSolarHour(const double solar_hour) {
  meteorology_.UpdateLocalSolarHour(solar_hour);
  UpdateSolarHour(meteorology_);
}

double PlantRadiation::CalculateExtinctionCoefficientForDirect(
    const double solar_elevation) {
  // Sun is below horizon
  if (solar_elevation < 0.00000001) {
    return 0.0f;
  }

  // Formula [3.3] in book p.55
  // k_dr = 0.5 / sin(β)
  return (1.0f / (2.0f * std::sin(solar_elevation)));
}

double PlantRadiation::CalculateExtinctionCoefficientForDiffuse() const {
  // TODO: move these hard-coded constants to constant variables

  // Formula [3.29] in book p.65
  // k_df = (1 + 0.1174 * sqrt(L)) / (1 + 0.3732 * sqrt(L))
  double sqrt_lai = std::sqrt(total_leaf_area_index_);
  return (1.0f + 0.1174 * sqrt_lai) / (1.0f + 0.3732 * sqrt_lai);
}

PlantRadiation::InterceptRadiance
PlantRadiation::CalculateInterceptHourlyRadiance(
    const double direct_radiation, const double extinction_coefficient_direct,
    const double diffuse_radiation,
    const double extinction_coefficient_diffuse) const {
  // reflection coefficient
  constexpr double p = 0.11f;
  // scatter correction
  constexpr double sqrt_alpha = std::sqrt(0.5);

  // Formula [3.8] in book p.58
  // I_i_dr = (1 - p) * I_dr * (1 - exp(-sqrt(α) * k_dr * L))
  double I_i_dr = (1.0f - p) * direct_radiation *
                  (1 - std::exp(-sqrt_alpha * extinction_coefficient_direct *
                                total_leaf_area_index_));

  // Formula [3.32] in book p.66
  // I_i_df = (1 - p) * I_df * (1 - exp(-sqrt(α) * k_df * L))
  double I_i_df = (1.0f - p) * diffuse_radiation *
                  (1 - std::exp(-sqrt_alpha * extinction_coefficient_diffuse *
                                total_leaf_area_index_));

  return {I_i_dr, I_i_df};
}

PlantRadiation::InterceptRadiance
PlantRadiation::CalculateInterceptDailyRadiance(
    const double solar_hour_sunrise, const double solar_hour_sunset) {
  // TODO: This should be filled in

  // It is sure that this function would be used, so having it here just to test
  // the dependency.
  UpdateSolarHour(0);

  return {0.0, 0.0};
}

PlantRadiation::AbsorbedPhotosyntheticallyActiveRadiation
PlantRadiation::CalculateAbsorbedHourPAR(
    const double direct_radiation, const double diffuse_radiation,
    const double extinction_coefficient_direct) const {
  // reflection coefficient
  constexpr double P = 0.04;
  // scatter coefficient
  constexpr double alpha = 0.8;
  // scatter correction
  constexpr double sqrt_alpha = std::sqrt(alpha);

  // Formula [3.1] in book p.52
  // τ_dr = exp(-k_dr * L)
  // L: leaf area index
  double tau_dr =
      std::exp(-extinction_coefficient_direct * total_leaf_area_index_);

  // Formula [3.6] in book p.57
  // τ_dr_α = exp(-sqrt(α) * k_dr * L)
  // L: leaf area index
  double tau_dr_alpha = std::exp(-sqrt_alpha * extinction_coefficient_direct *
                                 total_leaf_area_index_);

  // Formula [3.33] in book p.67
  // Q_p_dr = (1 - p) * tau_dr_alpha * Q_dr
  double total_direct_par = (1.0 - P) * tau_dr_alpha * direct_radiation;

  // Formula [3.34] in book p.67
  // Q_p_dr_dr = (1 - p) * tau_dr * Q_dr
  double direct_total_direct_par = (1.0 - P) * tau_dr * direct_radiation;

  // Formula [3.36] in book p.68
  // Q_p_dr_α = (Q_p_dr - Q_p_dr_dr) / 2
  double half_irradiance_scattered =
      (total_direct_par - direct_total_direct_par) / 2.0;

  // Formula [3.37] in book p.68
  // Q_p_df_bar = (1 - p) * Q_df * (1 - exp(-sqrt(α) * k_df * L)) /
  //              (sqrt(α) * k_df * L)
  double avg_diffuse_solar_irradiance =
      (1.0 - P) * diffuse_radiation *
      (1.0 - std::exp(-sqrt_alpha * kExtinctionCoefficientForDiffuse *
                      total_leaf_area_index_)) /
      (sqrt_alpha * kExtinctionCoefficientForDiffuse * total_leaf_area_index_);

  // Formula [3.38] in book p.68
  // Q_sl = α * (k_dr * Q_dr + Q_p_df_bar + Q_p_dr_α)
  double I_sunlit =
      alpha * (kExtinctionCoefficientForDiffuse * direct_radiation +
               avg_diffuse_solar_irradiance + half_irradiance_scattered);

  // Formula [3.39] in book p.69
  // Q_sh = α * (Q_p_df_bar + Q_p_dr_α)
  double I_shaded =
      alpha * (avg_diffuse_solar_irradiance + half_irradiance_scattered);

  return {I_sunlit, I_shaded};
}

PlantRadiation::LeafIndexArea PlantRadiation::CalculateLai(
    const double k_dr) const {
  // Formula [3.41] in book p.69
  // L_sl = (1 - exp(-k_dr * L)) / k_dr
  double lai_sl = 0.0;
  if (k_dr > 0.0) {
    lai_sl = (1 - std::exp(-k_dr * total_leaf_area_index_)) / k_dr;
  }

  // Formula [3.42] in book p.69
  // L_sh = L - L_sl
  double lai_sh = total_leaf_area_index_ - lai_sl;

  return {lai_sl, lai_sh};
}

}  // namespace environment