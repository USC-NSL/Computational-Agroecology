#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUNINFO_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUNINFO_H_

#include <chrono>
#include <tuple>

#include "environment/climate.h"
#include "environment/location.h"

namespace environment {

const double kPI = 3.14159265358979323846;
const double k2PI = 2.0 * kPI;
const double kPIforDegree = 180.0;
const double kTropic = 23.45;

const int kDaysPerYear = 365;
const int kHoursPerDay = 24;
const int kDaysLeftPerYear = 10;
const int kHoursHalfDay = 12;
const int kMinsPerHour = 60;
const int kSecsPerMin = 60;

const double kPiDividedBy12 = kPI / kHoursHalfDay;

class SunInfo {
 public:
  SunInfo(const std::chrono::system_clock::time_point &time,
          const Location &location, const Climate::ZoneType climate_zone);

  const double &solar_azimuth() const { return solar_azimuth_; }
  const double solar_inclination() const {
    // 90 degrees - solar_altitude
    return kPI / 2.0 - solar_altitude_;
  }
  const double &hourly_irradiance() const { return I_t_; }

  void SimulateToTime(const std::chrono::system_clock::time_point &time,
                      const Location &location,
                      const Climate::ZoneType climate_zone);

 private:
  double solar_azimuth_;
  double solar_altitude_;  // aka β

  // Local solar time for sunrise (hours)
  double t_sr_;
  // Local solar time for sunset (hours)
  double t_ss_;
  // Day length (hours)
  double day_length_;

  // Daily total solar irradiance on a horizontal surface (J m^-2 day^-1)
  double I_t_d_;
  // Daily diffuse irradiance on a horizontal surface
  double I_df_d_;
  // Daily direct irradiance on a horizontal surface
  double I_dr_d_;

  // Hourly solar irradiance on a horizontal surface (W m^-2)
  double I_t_;
  // Hourly diffuse irradiance on a horizontal surface
  double I_df_;
  // Hourly direct irradiance on a horizontal surface
  double I_dr_;

  // TODO: Air vapor pressure, wind speed, and air temperature haven't
  // implemented.

  static const double DegreeToRadians(const double degree);
  static const double RadiansToDegree(const double radians);

  void Update(const int t_d, const int hour, const double longitude,
              const double latitude, const Climate::ZoneType climate_zone);

  // Given the day of year, calculate solar declination δ in radians.
  double CalculateSolarDeclination(const int t_d);

  // Given the day of year, local time (hours), and observer's longitude,
  // convert local time (hours) to local solar time (hours).
  double CalculateLocalSolarTime(const int t_d, const int hour,
                                 const double longitude);

  // Given local solar time (hours), calculate the hour angle τ in radians.
  double CalculateHourAngle(const double t_h);

  // Solar position and time
  // Given solar declination δ, hour angle τ, and observer's latitude λ,
  // calculate solar altitude.
  double CalculateSolarAltitude(const double delta, const double tau,
                                const double lambda);

  // Given solar declination δ, observer's latitude λ, solar altitude β, and
  // local solar time, calculate solar azimuth.
  double CalculateSolarAzimuth(const double delta, const double lambda,
                               const double beta, const double t_h);

  // Day length and times of sunrise and sunset
  // Given solar declination δ, hour angle τ, and observer's latitude λ,
  // calculate local solar time for sunrise, local solar time for sunset, and
  // day length in hours.
  std::tuple<double, double, double> CalculateDayLength(const double delta,
                                                        const double tau,
                                                        const double lambda);

  // Solar irradiance
  // These two functions need other variables.
  // Given those variables, climate zone, and day length, calculate daily
  // total solar irradiance, daily diffuse irradiance, and daily direct
  // irradiance.
  std::tuple<double, double, double> CalculateDailySolarIrradiance(
      const double I_c_prime, const double a, const double b,
      const Climate::ZoneType climate_zone, const double day_length);

  // Given those variables, solar altitude β, daily total solar irradiance, and
  // local solar time, calculate hourly solar irradiance, hourly diffuse
  // irradiance, and hourly direct irradiance.
  std::tuple<double, double, double> CalculateHourlySolarIrradiance(
      const double beta, const double I_c_prime, const double a, const double b,
      const double I_t_d, const int t_h);
};

}  // namespace environment
#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUNINFO_H_