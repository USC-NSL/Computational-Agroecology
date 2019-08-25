#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENERGY_BALANCE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENERGY_BALANCE_H_

#include <utility>

#include "environment/meteorology.h"
#include "environment/plant_radiation.h"
#include "environment/soil.h"
#include "environment/weather.h"

namespace environment {

// Represents the amounts of energy on a single plant. Here we make the plant
// and the soil it is on fixed. Only time would change. This is not fully
// complete since they are some data that I have no idea how to get. Currently,
// I left them as to-dos.
class EnergyBalance {
 public:
  EnergyBalance(const Meteorology &meteorology,
                const PlantRadiation &plant_radiation, const Weather &weather,
                const double reference_height, const double plant_height,
                const double leaf_area_index, const double leaf_width,
                const Soil &soil);

  // Update the data in this class according to the given information.
  void Update(const Meteorology &meteorology,
              const PlantRadiation &plant_radiation, const Weather &weather);

  // Accessors
  inline double total_latent_heat_flux() const {
    return total_latent_heat_flux_;
  }
  inline double latent_heat_flux_soil() const { return latent_heat_flux_soil_; }
  inline double latent_heat_flux_crop() const { return latent_heat_flux_soil_; }
  inline double total_sensible_heat_flux() const {
    return total_latent_heat_flux_;
  }
  inline double sensible_heat_flux_soil() const {
    return sensible_heat_flux_soil_;
  }
  inline double sensible_heat_flux_crop() const {
    return sensible_heat_flux_crop_;
  }

 private:
  // Have a private `meteorology_` and a private `plant_radiation_` so that this
  // class may work independently. When calculating the daily heat fluxes, we
  // need to switch between different solar hours. Doing that requires the help
  // from `Meteorology` and `PlantRadiation`. I personally don't want to mess up
  // other things, so I have these private member variables here. These two
  // variables should only be used for the calculation of daily heat fluxes.
  Meteorology meteorology_;
  PlantRadiation plant_radiation_;

  // TODO: Maybe the reference height should not be stored here.
  const double kReferenceHeight;

  // Plant's height (meters)
  const double plant_height_;

  // Leaf area index (unit-less)
  const double leaf_area_index_;

  // Width of leaves (meters)
  const double leaf_width_;

  // Reference soil
  // This is the soil that the plant is on.
  const Soil &soil_;

  // Internal constants
  // Formula [4.53] in book p.88
  // k = 0.397 ± 0.010
  // k is typically taken as 0.4
  const double kVKConstant = 0.4f;

  struct InternalConstants {
    // Zero plant displacement (meters)
    double zero_plane_displacement;
    // Roughness length (meters)
    double roughness_length;
    double u_star;
  };

  InternalConstants internal_constants_;

  // The unit for all energy is W m^-2.
  double total_latent_heat_flux_;
  double latent_heat_flux_soil_;
  double latent_heat_flux_crop_;

  double total_sensible_heat_flux_;
  double sensible_heat_flux_soil_;
  double sensible_heat_flux_crop_;

  // Update the information according to the specified solar hour. This utilizes
  // the private `meteorology_` and `plant_radiation_`. This should only be used
  // by `UpdateDailyHeatFluxes()`.
  void UpdateSolarHour(const double solar_hour, const Weather &weather);

  // Doing the integration over the whole solar day. It needs to get the energy
  // values under different solar hours. This relies on the help of
  // `UpdateSolarHour()`.
  void UpdateDailyHeatFluxes(const Weather &weather);

  // Update the hourly energy values according to the given `meteorology` and
  // `plant_radiation`.
  void UpdateHourlyHeatFluxes(const Meteorology &meteorology,
                              const PlantRadiation &plant_radiation);

  // Given the wind speed (m s^-1) from the weather data, calculate all internal
  // constants for other functions to use.
  InternalConstants CalculateInternalConstants(const double wind_speed) const;

  // Given a temperature (°C), calculate the slope of the saturated vapor
  // pressure (mbar). This function is under the assumption that the difference
  // between T_r and T_0 is small.
  static double CalculateSlopeOfSaturatedVaporPressure(
      const double temperature);

  struct AeroResistances {
    double between_soil_canopy;
    double between_canopy_reference_height;
  };
  // Given the wind speed (m s^-1) from the weather data and soil information,
  // calculate a pair of aero resistances (s m^-1). One is between the reference
  // height and the mean canopy flow, and the other is between the mean canopy
  // flow and the below soil surface.
  AeroResistances CalculateAeroResistances(const double wind_speed,
                                           const Soil &soil) const;

  // Given the plant information, calculate the boundary layer resistance
  // (s m^-1).
  double CalculateBoundaryLayerResistance() const;

  // Given the total radiance (W m^-2) and some plant information, calculate the
  // canopy resistance (s m^-1).
  double CalculateCanopyResistance(double total_radiance) const;

  // Given the information about soil, calculate the soil resistance (s m^-1).
  static double CalculateSoilResistance(const Soil &soil);

  struct EnergySupply {
    double soil;
    double canopy;
  };

  // Given the hourly net radiance (W m^-2), extinction_coefficient_direct
  // (unit-less), and solar inclination (radians), calculate the energy
  // available to soil and canopy (W m^-2).
  EnergySupply CalculateEnergySupply(const double hour_net_radiance,
                                     const double extinction_coefficient_direct,
                                     const double solar_inclination) const;

  // Given the slope of the saturated vapor pressure (mbar K^-1), aero
  // resistances (s m^-1), boundary resistance (s m^-1), canopy resistance (s
  // m^-1), soil resistance (s m^-1), energy available to soil and canopy
  // (W m^-2), total available energy (W m^-2), the amount of heat to raise the
  // temperature of a unit volume of are by one Kelvin (J m^-3 K^-1), and vapor
  // pressure deficit (mbar), calculate the total latent heat flux (W m^-2).
  static double CalculateTotalLatentHeatFlux(
      const double slope_of_saturated_vapor_pressure,
      const AeroResistances &aero_resistances,
      const double boundary_layer_resistance, const double canopy_resistance,
      const double soil_resistance, const EnergySupply &energy_supply,
      const double total_available_energy, const double vapor_pressure_deficit);
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENERGY_BALANCE_H_