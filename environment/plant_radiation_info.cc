#include "plant_radiation_info.h"

#include <cmath>

namespace environment {

PlantRadiationInfo::PlantRadiationInfo(const double lai,
                                       const SunInfo &sun_info)
    : sun_info_(sun_info), total_lai_(lai), k_df_(CalculateKDf()) {
  Update(sun_info_);
}

void PlantRadiationInfo::Update(const SunInfo &sun_info) {
  I_i_d_ = CalculateInterceptDailyRadiance(sun_info.t_sr_, sun_info.t_ss_);
  UpdateSolarHour(sun_info);
}

void PlantRadiationInfo::Update(const int day_of_year, const double t_h) {
  // Cautious: we are using the private update function.
  sun_info_.UpdateDayOfYear(day_of_year);
  // Cautious: we are using the private update function.
  sun_info_.UpdateLocalSolarHour(t_h);
  Update(sun_info_);
}

void PlantRadiationInfo::UpdateSolarHour(const SunInfo &sun_info) {
  double k_dr = CalculateKDr(sun_info.solar_elevation());

  I_i_ = CalculateInterceptHourlyRadiance(sun_info.I_dr_, k_dr, sun_info.I_df_,
                                          k_df_);

  AbsorbedPAR par =
      CalculateAbsorbedHourPAR(sun_info.I_dr_, sun_info.I_df_, k_dr);
  I_sunlit_ = par.sunlit;
  I_shaded_ = par.shaded;

  Lai lai = CalculateLai(k_dr);
  lai_sunlit_ = lai.sunlit;
  lai_shaded_ = lai.shaded;
}

void PlantRadiationInfo::UpdateSolarHour(const double t_h) {
  sun_info_.UpdateLocalSolarHour(t_h);
  UpdateSolarHour(sun_info_);
}

double PlantRadiationInfo::CalculateKDr(const double solar_elevation) {
  // Sun is below horizon
  if (solar_elevation < 0.00000001) {
    return 0.0;
  }

  // Formula [3.3] in book p.55
  // k_dr = 0.5 / sin(β)
  return (1 / (2 * std::sin(solar_elevation)));
}

double PlantRadiationInfo::CalculateKDf() const {
  // Formula [3.29] in book p.65
  // k_df = (1 + 0.1174 * sqrt(L)) / (1 + 0.3732 * sqrt(L))
  double sqrt_L = std::sqrt(total_lai_);
  return (1.0f + 0.1174 * sqrt_L) / (1.0f + 0.3732 * sqrt_L);
}

PlantRadiationInfo::InterceptRadiance
PlantRadiationInfo::CalculateInterceptHourlyRadiance(const double I_dr,
                                                     const double k_dr,
                                                     const double I_df,
                                                     const double k_df) const {
  // reflection coefficient
  double p = 0.11f;
  // scatter correction
  double sqrt_alpha = std::sqrt(0.5);

  // Formula [3.8] in book p.58
  // I_i_dr = (1 - p) * I_dr * (1 - exp(-sqrt(α) * k_dr * L))
  double I_i_dr =
      (1.0f - p) * I_dr * (1 - std::exp(-sqrt_alpha * k_dr * total_lai_));

  // Formula [3.32] in book p.66
  // I_i_df = (1 - p) * I_df * (1 - exp(-sqrt(α) * k_df * L))
  double I_i_df =
      (1.0f - p) * I_df * (1 - std::exp(-sqrt_alpha * k_df * total_lai_));

  return {I_i_dr, I_i_df};
}

PlantRadiationInfo::InterceptRadiance
PlantRadiationInfo::CalculateInterceptDailyRadiance(const double t_sr,
                                                    const double t_ss) {
  // TODO: This should be filled in

  // It is sure that this function would be used, so having it here just to test
  // the dependency.
  UpdateSolarHour(0);

  return {0.0, 0.0};
}

PlantRadiationInfo::AbsorbedPAR PlantRadiationInfo::CalculateAbsorbedHourPAR(
    const double I_dr, const double I_df, const double k_dr) const {
  // reflection coefficient
  double P = 0.04;
  // scatter coefficient
  double alpha = 0.8;
  // scatter correction
  double sqrt_alpha = std::sqrt(alpha);

  // Formula [3.1] in book p.52
  // τ_dr = exp(-k_dr * L)
  // L: leaf area index
  double tau_dr = std::exp(-k_dr * total_lai_);

  // Formula [3.6] in book p.57
  // τ_dr_α = exp(-sqrt(α) * k_dr * L)
  // L: leaf area index
  double tau_dr_alpha = std::exp(-sqrt_alpha * k_dr * total_lai_);

  // Formula [3.33] in book p.67
  // Q_p_dr = (1 - p) * tau_dr_alpha * Q_dr
  double I_p_dr = (1.0 - P) * tau_dr_alpha * I_dr;

  // Formula [3.34] in book p.67
  // Q_p_dr_dr = (1 - p) * tau_dr * Q_dr
  double I_p_dr_dr = (1.0 - P) * tau_dr * I_dr;

  // Formula [3.36] in book p.68
  // Q_p_dr_α = (Q_p_dr - Q_p_dr_dr) / 2
  double I_p_dr_alpha = (I_p_dr - I_p_dr_dr) / 2.0;

  // Formula [3.37] in book p.68
  // Q_p_df_bar = (1 - p) * Q_df * (1 - exp(-sqrt(α) * k_df * L)) /
  //              (sqrt(α) * k_df * L)
  double I_p_df_bar = (1.0 - P) * I_df *
                      (1.0 - std::exp(-sqrt_alpha * k_df_ * total_lai_)) /
                      (sqrt_alpha * k_df_ * total_lai_);

  // Formula [3.38] in book p.68
  // Q_sl = α * (k_dr * Q_dr + Q_p_df_bar + Q_p_dr_α)
  double I_sunlit = alpha * (k_dr * I_dr + I_p_df_bar + I_p_dr_alpha);

  // Formula [3.39] in book p.69
  // Q_sh = α * (Q_p_df_bar + Q_p_dr_α)
  double I_shaded = alpha * (I_p_df_bar + I_p_dr_alpha);

  return {I_sunlit, I_shaded};
}

PlantRadiationInfo::Lai PlantRadiationInfo::CalculateLai(
    const double k_dr) const {
  // Formula [3.41] in book p.69
  // L_sl = (1 - exp(-k_dr * L)) / k_dr
  double L_sl = 0.0;
  if (k_dr > 0.0) {
    L_sl = (1 - std::exp(-k_dr * total_lai_)) / k_dr;
  }

  // Formula [3.42] in book p.69
  // L_sh = L - L_sl
  double L_sh = total_lai_ - L_sl;

  return {L_sl, L_sh};
}

}  // namespace environment