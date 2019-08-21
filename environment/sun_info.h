#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_INFO_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_INFO_H_

#include <chrono>
#include <tuple>

#include "environment/climate.h"
#include "environment/location.h"
#include "environment/weather.h"

constexpr double kPI = 3.14159265358979323846;
constexpr double k2PI = 2.0 * kPI;
constexpr double kPIforDegree = 180.0;

constexpr int kDaysPerYear = 365;
constexpr int kHoursPerDay = 24;
constexpr int kHoursHalfDay = kHoursPerDay / 2;
constexpr int kMinsPerHour = 60;
constexpr int kMinsPerDay = kMinsPerHour * kHoursPerDay;
constexpr int kSecsPerMin = 60;
constexpr int kSecsPerHour = kSecsPerMin * kMinsPerHour;
constexpr int kSecsPerDay = kSecsPerMin * kMinsPerHour * kHoursPerDay;
constexpr int kDaysLeftPerYear = 10;

// constant variables only for this file
namespace {

constexpr double kTropic = 23.45;
constexpr double kPiDividedBy12 = kPI / kHoursHalfDay;

}  // namespace

namespace environment {

// Forward declaration
class PlantRadiation;

// The public interfaces are mainly for showing the current states of the sun.
class SunInfo {
 public:
  SunInfo(const std::chrono::system_clock::time_point &time,
          const Location &location, const Climate::ZoneType climate_zone,
          const Weather &weather);

  const double &solar_azimuth() const { return solar_azimuth_; }
  const double &solar_elevation() const { return solar_elevation_; }
  const double solar_inclination() const {
    // 90 degrees - solar_altitude
    return kPI / 2.0 - solar_elevation_;
  }
  const double &hourly_irradiance() const { return I_t_; }
  const double &hourly_diffuse_irradiance() const { return I_df_; }
  const double &hourly_direct_irradiance() const { return I_dr_; }

  const double &saturated_vapor_pressure() const { return e_s_T_a_; }
  const double &air_temperature() const { return T_a_; }

  void Update(const std::chrono::system_clock::time_point &time,
              const Weather &weather);
  void Update(const int day_of_year, const int hour, const int minute,
              const int second, const Weather &weather);

  // This is designed to be used by `class PlantRadiationInfo`. We need this
  // when calculating the daily intercepted of total solar radiation. This
  // function does not update any information in this function. It only uses
  // some variables in `ConstantCaches` to help itself.
  // Given local solar time, calculate hourly solar irradiance, hourly diffuse
  // irradiance, and hourly direct irradiance.
  std::tuple<double, double, double> CalculateHourlySolarIrradiance(
      const double t_h) const;

 private:
  friend class PlantRadiation;

  // Information binded to the current geographic location
  const Location &geo_location_;
  const Climate::ZoneType &climate_zone_;

  // Information about the current date and time
  double day_of_year_;
  double local_solar_hour_;

  // This stores some caches of constants related to geographic information and
  // day of a year. These variables are useful when we need to frequently call
  // `CalculateHourlySolarIrradiance()`.
  struct ConstantCaches {
    // Constants related to geographic information
    // Observer's latitude λ
    double lambda;

    // Constants related to day of a year
    // Solar declination δ (radians)
    double delta;

    // Constants used to calculate irradiance
    double I_c_prime;
    // To solve integral sin(β), we need the following a and b
    double a;
    double b;
  };

  ConstantCaches constant_caches_;

  // TODO: The following may need refactor. It may not need so many private
  // variables. Keeping them just in case some functions may use them. The
  // refactor should start when the first version of the whole plant growth
  // model finishs.
  double solar_azimuth_;

  // Solar angle from horizontal (aka β) in radians
  double solar_elevation_;

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

  // Saturated vapor pressure (mbar)
  double e_s_T_a_;

  // TODO: In the book, the wind speed is assumed constant. However, we don't
  // have the data for wind speed. Just leave it blank for now.

  // Air temperature
  double T_a_;

  // Update variables related to geographic data. This should only be called
  // once during construction because we assume the geographic location and
  // climate remain unchanged.
  void UpdateGeoData();

  // We separate these functions on purpose. When simulating the plant growth,
  // it is likely that we do not need to update all the information but a factor
  // among them. Any class or function needs these should be a friend of this
  // class because using them wrongly may cause data inconsistency. These
  // interfaces may be revised in future refactor.
  void UpdateDayOfYear(const int day_of_year);
  void UpdateLocalTime(const int hour, const int minute, const int second);
  void UpdateLocalSolarHour(const double t_h);
  void UpdateWeather(const Weather &weather);

  // These functions below are static, which means they are independent to
  // member variables. We use these functions to calculate member variables in
  // this class.
  static double DegreeToRadians(const double degree);
  static double RadiansToDegree(const double radians);

  // Given the day of year, calculate solar declination δ in radians.
  static double CalculateSolarDeclination(const int t_d);

  // Given the day of year, local time (hours), and observer's longitude,
  // convert local time (hours) to local solar time (hours).
  static double CalculateLocalSolarTime(const int t_d, const double hour,
                                        const double longitude);

  // Given local solar time (hours), calculate the hour angle τ in radians.
  static double CalculateHourAngle(const double t_h);

  // Given solar declination δ, hour angle τ, and observer's latitude λ,
  // calculate solar angle from horizontal (radians).
  static double CalculateSolarElevation(const double delta, const double tau,
                                        const double lambda);

  // Given solar declination δ, observer's latitude λ, solar altitude β, and
  // local solar time, calculate solar azimuth.
  static double CalculateSolarAzimuth(const double delta, const double lambda,
                                      const double beta, const double t_h);

  // Day length and times of sunrise and sunset
  // Given solar declination δ, hour angle τ, and observer's latitude λ,
  // calculate local solar time for sunrise, local solar time for sunset, and
  // day length in hours.
  static std::tuple<double, double, double> CalculateDayLength(
      const double delta, const double lambda);

  // Solar irradiance
  // These two functions need other variables.
  // Given those variables, climate zone, and day length, calculate daily
  // total solar irradiance, daily diffuse irradiance, and daily direct
  // irradiance.
  static std::tuple<double, double, double> CalculateDailySolarIrradiance(
      const double I_c_prime, const double a, const double b,
      const Climate::ZoneType climate_zone, const double day_length);

  // Given those variables, solar altitude β, daily total solar irradiance, and
  // local solar time, calculate hourly solar irradiance, hourly diffuse
  // irradiance, and hourly direct irradiance.
  static std::tuple<double, double, double> CalculateHourlySolarIrradiance(
      const double beta, const double I_c_prime, const double a, const double b,
      const double I_t_d, const double t_h);

  static double CalculateSaturatedVaporPressure(const double temperature);

  static double CalculateAirTemperature(double t_h, const double temp_min,
                                        const double temp_max,
                                        const double t_sr, const double t_ss);
};

}  // namespace environment
#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_INFO_H_