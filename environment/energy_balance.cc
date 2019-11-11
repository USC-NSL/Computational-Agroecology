#include "energy_balance.h"

#include <cmath>

namespace environment {

EnergyBalance::EnergyBalance(const Meteorology &meteorology,
                             const PlantRadiation &plant_radiation,
                             const Weather &weather,
                             const double reference_height,
                             const double plant_height,
                             const double leaf_area_index,
                             const double leaf_width, const Soil &soil)
    : meteorology_(meteorology),
      plant_radiation_(plant_radiation),
      kReferenceHeight(reference_height),
      plant_height_(plant_height),
      leaf_area_index_(leaf_area_index),
      leaf_width_(leaf_width),
      soil_(soil) {
  Update(meteorology, plant_radiation, weather);
}

void EnergyBalance::Update(const Meteorology &meteorology,
                           const PlantRadiation &plant_radiation,
                           const Weather &weather) {
  internal_constants_ = CalculateInternalConstants(meteorology.wind_speed());
  UpdateDailyHeatFluxes(weather);
  UpdateHourlyHeatFluxes(meteorology, plant_radiation);
}

void EnergyBalance::UpdateSolarHour(const double solar_hour,
                                    const Weather &weather) {
  meteorology_.UpdateLocalSolarHour(solar_hour);
  meteorology_.UpdateHourlyNetRadiation(weather);
  plant_radiation_.UpdateSolarHour(meteorology_);
  UpdateHourlyHeatFluxes(meteorology_, plant_radiation_);
}

void EnergyBalance::UpdateDailyHeatFluxes(const Weather &weather) {
  // TODO: fill in this function

  // Calling `UpdateSolarHour()` just to indicate the dependency on the
  // `UpdateSolarHour()` function.
  UpdateSolarHour(0, weather);
}

void EnergyBalance::UpdateHourlyHeatFluxes(
    const Meteorology &meteorology, const PlantRadiation &plant_radiation) {
  // This is denoted as Δ in the book.
  double slope_of_saturated_vapor_pressure =
      CalculateSlopeOfSaturatedVaporPressure(meteorology.air_temperature());

  // This is mentioned in the book p.77.
  constexpr double gamma = 0.658;

  // They are denoted as r_a_s and r_a_a in the book.
  AeroResistances aero_resistances =
      CalculateAeroResistances(meteorology.wind_speed(), soil_);

  // This is denoted as r_a_c in the book.
  double boundary_layer_resistance = CalculateBoundaryLayerResistance();

  // This is denoted as r_s_c in the book.
  double canopy_resistance =
      CalculateCanopyResistance(meteorology.hourly_total_irradiance());

  // This is denoted as r_s_s in the book.
  double soil_resistance = CalculateSoilResistance(soil_);

  // They are denoted as A_c and A_s in the book.
  EnergySupply energy_supply = CalculateEnergySupply(
      meteorology.hourly_net_radiation_,
      plant_radiation.CalculateExtinctionCoefficientForDirect(
          meteorology.local_solar_hour_),
      meteorology.solar_inclination());

  // This is denoted as A in the book.
  double total_available_energy = energy_supply.canopy + energy_supply.soil;

  // This is denoted as ρ * c_p in the book. It is mentioned in book p.77.
  constexpr double heat_to_raise_one_Kelvin_unit_volume = 1221.09;

  // Formula [4.15] in book p.79
  // D = e_s(T_r) - e_r
  double vapor_pressure_deficit = meteorology.saturated_vapor_pressure() -
                                  meteorology.actual_vapor_pressure();

  // This is denoted as λET in the book.
  total_latent_heat_flux_ = CalculateTotalLatentHeatFlux(
      slope_of_saturated_vapor_pressure, aero_resistances,
      boundary_layer_resistance, canopy_resistance, soil_resistance,
      energy_supply, total_available_energy, vapor_pressure_deficit);

  // Formula [4.42] in book p.84
  // D_0 = D + (r_a_a / (ρ * c_p)) * (Δ * A - (Δ + γ) * λET)
  double vapor_pressure_deficit_mean_canopy =
      vapor_pressure_deficit +
      (aero_resistances.between_canopy_reference_height /
       heat_to_raise_one_Kelvin_unit_volume) *
          (slope_of_saturated_vapor_pressure * total_available_energy -
           (slope_of_saturated_vapor_pressure + gamma) *
               total_latent_heat_flux_);

  double numerator, denominator;
  // Formula [4.43] in book p.84
  // λET_s = ((Δ * A_s) + (ρ * c_p * D_0 / r_a_s)) / (Δ + γ * (r_s_s + r_a_s) /
  // r_a_s)
  numerator = (slope_of_saturated_vapor_pressure * energy_supply.soil) +
              ((heat_to_raise_one_Kelvin_unit_volume *
                vapor_pressure_deficit_mean_canopy) /
               aero_resistances.between_soil_canopy);
  denominator =
      slope_of_saturated_vapor_pressure +
      (gamma * (soil_resistance + aero_resistances.between_soil_canopy) /
       aero_resistances.between_soil_canopy);
  latent_heat_flux_soil_ = numerator / denominator;

  // Formula [4.43] in book p.84
  // λET_c = ((Δ * A_c) + (ρ * c_p * D_0 / r_a_c)) / (Δ + γ * (r_s_c + r_a_c) /
  // r_a_c))
  numerator = (slope_of_saturated_vapor_pressure * energy_supply.canopy) +
              ((heat_to_raise_one_Kelvin_unit_volume *
                vapor_pressure_deficit_mean_canopy) /
               boundary_layer_resistance);
  denominator = slope_of_saturated_vapor_pressure +
                (gamma * (canopy_resistance + boundary_layer_resistance) /
                 boundary_layer_resistance);
  latent_heat_flux_crop_ = numerator / denominator;

  // Formula [4.44] in book p.84
  // H_s = (γ * A_s * (r_s_s + r_a_s) - ρ * c_p * D_0) / (Δ * r_a_s + γ * (r_s_s
  // + r_a_s))
  numerator = gamma * energy_supply.soil *
                  (soil_resistance + aero_resistances.between_soil_canopy) -
              (heat_to_raise_one_Kelvin_unit_volume *
               vapor_pressure_deficit_mean_canopy);
  denominator =
      (slope_of_saturated_vapor_pressure *
       aero_resistances.between_soil_canopy) +
      (gamma * (soil_resistance + aero_resistances.between_soil_canopy));
  sensible_heat_flux_soil_ = numerator / denominator;

  // Formula [4.44] in book p.84
  // H_c = (γ * A_c * (r_s_c + r_a_c) - ρ * c_p * D_0) / (Δ * r_a_c + γ * (r_s_c
  // + r_a_c))
  numerator = gamma * energy_supply.canopy *
                  (canopy_resistance + boundary_layer_resistance) -
              (heat_to_raise_one_Kelvin_unit_volume *
               vapor_pressure_deficit_mean_canopy);
  denominator =
      (slope_of_saturated_vapor_pressure * boundary_layer_resistance) +
      (gamma * (canopy_resistance + boundary_layer_resistance));
  sensible_heat_flux_crop_ = numerator / denominator;
  total_sensible_heat_flux_ =
      sensible_heat_flux_crop_ + sensible_heat_flux_soil_;
}

EnergyBalance::InternalConstants EnergyBalance::CalculateInternalConstants(
    const double wind_speed) const {
  // Formula [4.62] in book p.92
  // d = 0.64 * h
  double zero_plane_displacement = 0.64f * plant_height_;

  // Formula [4.62] in book p.92
  // z_0 = 0.13 * h
  double roughness_length = 0.13f * plant_height_;

  // Formula [4.67] in book p.94
  // u_star = k * u(z) / ln((z - d) / z_0)
  double friction_velocity =
      kVKConstant * wind_speed /
      std::log((plant_height_ - zero_plane_displacement) / roughness_length);

  return {zero_plane_displacement, roughness_length, friction_velocity};
}

double EnergyBalance::CalculateSlopeOfSaturatedVaporPressure(
    const double temperature) {
  // Formula [4.18] in book p.79
  // Δ ≈ de_s(T)/dT = 25029.4 * exp(17.269 * T / (T + 237.3)) / (T + 237.3)^2
  double numerator =
      25029.4f * std::exp(17.269 * temperature / (temperature + 237.3f));
  double denominator = (temperature + 237.3f);
  denominator *= denominator;
  return numerator / denominator;
}

EnergyBalance::AeroResistances EnergyBalance::CalculateAeroResistances(
    const double wind_speed, const Soil &soil) const {
  // TODO: these variables should be set
  double n;  // attenuation coefficient for eddy diffusivity

  // The roughness length of the soil
  // TODO: Should be depend on the texture of soil. Table is in p.92.
  double roughness_lengths_soil;

  // Formula [4.65] in book p.94
  // K(h) = k * u_* * h
  double eddy_diffusivity_canopy_top =
      kVKConstant * internal_constants_.friction_velocity * plant_height_;

  // Formula [4.68] in book p.94
  // r_a_s = h * exp(n) / (n * K(h)) * (exp(-n * z_s0 / h) -
  // exp(-n * (z_0 + d) / h))
  double aero_resistance_soil_canopy =
      plant_height_ * std::exp(n) / (n * eddy_diffusivity_canopy_top) *
      (exp(-n * roughness_lengths_soil / plant_height_) -
       exp(-n *
           (internal_constants_.roughness_length +
            internal_constants_.zero_plane_displacement) /
           plant_height_));

  // Formula [4.69] in book p.94
  // r_a_a = (1 / (k * u_star)) * ln((z_r - d) / (h - d)) + (h / (n * K(h))) *
  // (exp(n * (1 - ((z_0 + d) / h))) - 1)
  double aero_resistance_canopy_reference_height =
      (1.0f / (kVKConstant * internal_constants_.friction_velocity)) *
          std::log(
              (kReferenceHeight - internal_constants_.zero_plane_displacement) /
              (plant_height_ - internal_constants_.zero_plane_displacement)) +
      ((plant_height_ / (n * eddy_diffusivity_canopy_top)) *
       (std::exp(n * (1.0f - (internal_constants_.roughness_length +
                              internal_constants_.zero_plane_displacement) /
                                 plant_height_)) -
        1.0f));

  return {aero_resistance_soil_canopy, aero_resistance_canopy_reference_height};
}

double EnergyBalance::CalculateBoundaryLayerResistance() const {
  // TODO: fill in this variable
  // This variable depends on the type of plant. Check out the table in p.93.
  double wind_attenuation_coefficient;

  // Formula [4.63] in book p.93
  // u(h) = u_star / k * ln((h - d) / z_0)
  double wind_speed_canopy_top =
      internal_constants_.friction_velocity / kVKConstant *
      std::log((plant_height_ - internal_constants_.zero_plane_displacement) /
               internal_constants_.zero_plane_displacement);

  // Formula [4.78] in book p.97
  // r_a_c = α / (0.012 * L * (1 - exp(-α / 2)) * sqrt(u(h) / w))
  return (wind_attenuation_coefficient /
          (0.012f * leaf_area_index_ *
           (1 - std::exp(-wind_attenuation_coefficient / 2.0f)) *
           std::sqrt(wind_speed_canopy_top / leaf_width_)));
}

double EnergyBalance::CalculateCanopyResistance(double total_radiance) const {
  // TODO: fill in this variable
  // These two variables depend on the type of plant. They are mentioned in book
  // p.97.
  double a_1, a_2;

  // Avoid dividing zero
  if (total_radiance <= 0.0) {
    total_radiance = 0.01;
  }

  double irradiance_PAR = total_radiance / 2;

  // Formula [4.79] in book p.97
  // r_st = (a_1 + I_PAR) / (a_2 * I_PAR)
  double stomatal_resistance = (a_1 + irradiance_PAR) / (a_2 * irradiance_PAR);

  if (leaf_area_index_ > 0.5f * kTypicalMaxLeafIndexArea) {
    // Formula [4.80] in book p.98
    // r_s_c = r_st / (0.5 * L_cr) for L > 0.5L_cr
    return stomatal_resistance / (0.5f * kTypicalMaxLeafIndexArea);
  } else {
    // Formula [4.80] in book p.98
    // r_s_c = r_st / L for L <= 0.5L_cr
    return stomatal_resistance / leaf_area_index_;
  }
}

double EnergyBalance::CalculateSoilResistance(const Soil &soil) {
  // TODO: fill in these variables. These variables seem to have to be acquired
  // from some data source. There is no easy way for this moment to retrieve
  // these variables in our model.
  // This is denoted as λ in the book.
  double pore_size_distribution_index;
  // This is denoted as Θ_v in the book.
  double soil_water_volumetric_content;
  // This is denoted as Θ_v,sat in the book.
  double volumetric_water_content_at_soil_saturation;

  // Thickness of the dry soil which is denoted as l in book
  // TODO: The book just takes this as 0.02. Maybe we could make some changes on
  // this.
  double thickness_dry_soil = 0.02;

  // Soil's total porosity which is denoted as Φ_p in book
  // TODO: fill this
  double soil_total_porosity;

  // Formula [4.85] in book p.100
  // r_s,dry_s = (τl) / (Φ_p * D_m,v)
  double resistance_transfer_dry_soil =
      (kTortuositySoils * thickness_dry_soil) /
      (soil_total_porosity * kDiffusionCoefficientVaporInAir);

  // Empirical coefficient denoting how quickly soil resistance decreases with
  // increasing soil water content
  //
  // Formula [4.89] in book p.101
  // β = 1 / λ
  double beta = 1.0f / pore_size_distribution_index;

  // Formula [4.86] in book p.100
  // r_s_s(Θ_v) = r_s,dry_s * exp(-β * (Θ_v / Θ_v,sat))
  return resistance_transfer_dry_soil *
         std::exp(-beta * (soil_water_volumetric_content /
                           volumetric_water_content_at_soil_saturation));
}

EnergyBalance::EnergySupply EnergyBalance::CalculateEnergySupply(
    const double hour_net_radiance, const double extinction_coefficient_direct,
    const double solar_inclination) const {
  double radiance_soil = 0.0f, radiance_canopy = 0.0f;
  if (hour_net_radiance > 0.0) {
    // This is mentioned in book p.57.
    constexpr double alpha = 0.5f;

    // Formula [3.6] in book p.57
    // τ_dr_α = exp(-sqrt(α) * k_dr * L)
    double penetration_function_direct = std::exp(
        -std::sqrt(alpha) * extinction_coefficient_direct * leaf_area_index_);

    // Formula [4.30] in book p.82
    // A_c = (1 - τ_dr_α) * R_n
    radiance_canopy = hour_net_radiance * (1.0 - penetration_function_direct);

    // Formula [4.32] in book p.83
    // G = 0.35 * (τ_dr_α * Rn) * cos(θ)
    double ground_heat_flex_density = 0.35f * penetration_function_direct *
                                      hour_net_radiance *
                                      std::cos(solar_inclination);

    // Formula mentioned in book p.82
    // A_s = R_n - G - A_c
    radiance_soil =
        hour_net_radiance - radiance_canopy - ground_heat_flex_density;
  }

  return {radiance_soil, radiance_canopy};
}

double EnergyBalance::CalculateCanopyTemperature(double solar_hour, double sensible_heat_total, double sensible_heat_canopy)
{
   AeroResistances aero_resistances =
      CalculateAeroResistances(meteorology_.wind_speed(), soil_);
   double temperature_at_surface = ((sensible_heat_total / KHeatToRaiseOneKelvinUnitVolume) * aero_resistances.between_canopy_reference_height) + 
      meteorology_.air_temperature();
  // TODO: Why does this function not take wind_speed as a parameter?
   double boundary_layer_resistance = CalculateBoundaryLayerResistance();
   return ((sensible_heat_canopy / KHeatToRaiseOneKelvinUnitVolume) * boundary_layer_resistance) + temperature_at_surface;
}

double EnergyBalance::CalculateTotalLatentHeatFlux(
    const double slope_of_saturated_vapor_pressure,
    const EnergyBalance::AeroResistances &aero_resistances,
    const double boundary_layer_resistance, const double canopy_resistance,
    const double soil_resistance,
    const EnergyBalance::EnergySupply &energy_supply,
    const double total_available_energy, const double vapor_pressure_deficit) {
  // The following variables R_a, R_c, R_s, C_c, C_s, PM_c, and PM_s have no
  // meanings. They just represent sub-equations of a huge complicated equation.

  // Formula [4.38] in book p.83
  // R_a = (Δ + γ) * r_a_a
  double R_a = (slope_of_saturated_vapor_pressure + kPsychometricConstant) *
               aero_resistances.between_soil_canopy;

  // Formula [4.39] in book p.83
  // R_c = (Δ + γ) * r_a_c + γ * r_s_c
  double R_c = (slope_of_saturated_vapor_pressure + kPsychometricConstant) *
                   boundary_layer_resistance +
               kPsychometricConstant * canopy_resistance;

  // Formula [4.40] in book p.83
  // R_s = (Δ + γ) * r_a_s + γ * r_s_s
  double R_s = (slope_of_saturated_vapor_pressure + kPsychometricConstant) *
                   aero_resistances.between_soil_canopy +
               kPsychometricConstant * soil_resistance;

  // Formula [4.36] in book p.83
  // C_c = (1 + R_c * R_a / (R_s * (R_c + R_a)))^-1
  double C_c = 1.0f / (1.0f + (R_c * R_a / (R_s * (R_c + R_a))));

  // Formula [4.37] in book p.83
  // C_s = (1 + R_s * R_a / (R_c * (R_s + R_a)))^-1
  double C_s = 1.0f / (1.0f + (R_s * R_a / (R_c * (R_s + R_a))));

  // Formula [4.34] in book p.83
  // PM_c = (Δ * A + ((ρ * c_p * D - Δ * r_a_c * A_s) / (r_a_a + r_a_c))) / (Δ +
  // γ * ((1 + r_s_c) / (r_a_a + r_a_c)))
  double numerator, denominator;
  numerator = (slope_of_saturated_vapor_pressure * total_available_energy) +
              ((KHeatToRaiseOneKelvinUnitVolume * vapor_pressure_deficit -
                slope_of_saturated_vapor_pressure * boundary_layer_resistance *
                    energy_supply.soil) /
               (aero_resistances.between_canopy_reference_height +
                boundary_layer_resistance));
  denominator = slope_of_saturated_vapor_pressure +
                kPsychometricConstant *
                    (1.0 + (canopy_resistance /
                            (aero_resistances.between_canopy_reference_height +
                             boundary_layer_resistance)));
  double PM_c = numerator / denominator;

  // Formula [4.35] in book p.83
  // PM_s = (Δ * A + ((ρ * c_p * D - Δ * r_a_s * A_c) / (r_a_s + r_a_c))) / (Δ +
  // γ * (1 + (r_s_s / (r_a_s + r_a_s))))
  numerator =
      (slope_of_saturated_vapor_pressure * total_available_energy) +
      ((KHeatToRaiseOneKelvinUnitVolume * vapor_pressure_deficit -
        slope_of_saturated_vapor_pressure *
            aero_resistances.between_soil_canopy * energy_supply.canopy) /
       (aero_resistances.between_canopy_reference_height +
        aero_resistances.between_soil_canopy));

  denominator = slope_of_saturated_vapor_pressure +
                kPsychometricConstant *
                    (1.0f + (soil_resistance /
                             (aero_resistances.between_canopy_reference_height +
                              aero_resistances.between_soil_canopy)));
  double PM_s = numerator / denominator;

  // Formula [4.33] in book p.83
  // λET = C_c * PM_c + C_s * PM_s
  return C_c * PM_c + C_s * PM_s;
}

}  // namespace environment