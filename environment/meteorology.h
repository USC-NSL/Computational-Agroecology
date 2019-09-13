#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_METEOROLOGY_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_METEOROLOGY_H_

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
class EnergyBalanceInfo;
class PlantRadiation;

// Represents information about the meteorology, such as the Sun, vapor
// pressure, wind speed, and air temperature.
//
// All member function names with prefix "Calculate" do not modify this class.
// They only output values given paramters. On the other hand, member function
// names with prefix "Update" use those functions with "Calculate" to update the
// member variables in this class.
class Meteorology {
 public:
  // Given sufficient information, present the state about meteorology at a
  // single specific moment.
  Meteorology(const std::chrono::system_clock::time_point &local_time,
              const Location &location, const Climate::ZoneType climate_zone,
              const Weather &weather);

  // Modifiers
  // Update the information in this class by giving a timepoint.
  void Update(const std::chrono::system_clock::time_point &local_time,
              const Weather &weather);
  void Update(const int day_of_year, const int local_hour,
              const int local_minute, const int local_second,
              const Weather &weather);

  // Accessors
  const double &solar_azimuth() const { return solar_azimuth_; }
  const double &solar_elevation() const { return solar_elevation_; }
  const double solar_inclination() const {
    // 90 degrees - solar_altitude
    return kPI / 2.0 - solar_elevation_;
  }
  const double &hourly_total_irradiance() const {
    return hourly_solar_irradiance_.total;
  }
  const double &hourly_direct_irradiance() const {
    return hourly_solar_irradiance_.direct;
  }
  const double &hourly_diffuse_irradiance() const {
    return hourly_solar_irradiance_.diffuse;
  }
  const double &actual_vapor_pressure() const { return vapor_pressure_.actual; }
  const double &saturated_vapor_pressure() const {
    return vapor_pressure_.saturated;
  }
  const double &air_temperature() const { return air_temperature_; }

  double wind_speed() const { return wind_speed_; }

  // This stores some caches of constants related to geographic information and
  // day of a year. These variables are useful when we need to frequently call
  // `CalculateHourlySolarIrradiance()`.
  struct ConstantCaches {
    // Constants related to geographic information
    // Observer's latitude λ (radians)
    double observer_latitude;

    // Constants related to day of a year
    // Solar declination δ (radians)
    double solar_declination;

    // Constants used to calculate irradiance
    double I_c_prime;
    // To solve integral sin(β), we need the following a and b
    double a;
    double b;
  };

 private:
  friend class PlantRadiation;
  friend class EnergyBalance;

  struct SolarIrradiance {
    // Total solar irradiance on a horizontal surface
    double total;
    // Direct solar irradiance on a horizontal surface
    double direct;
    // Diffuse solar irradiance on a horizontal surface
    double diffuse;
  };

  struct VaporPressure {
    // Saturated vapor pressure (mbar)
    // This is denoted as e_s(T_a) in the book.
    double saturated;

    // Actual vapor pressure (mbar)
    // This is denoted as e_a in the book.
    double actual;
  };

  struct DayLengthAndTimesSunriseSunset {
    double solar_hour_sunrise;
    double solar_hour_sunset;
    double day_length;
  };

  // Information binded to the current geographic location
  const Location &geo_location_;
  const Climate::ZoneType &climate_zone_;

  // Information about the current date and time
  double day_of_year_;
  double local_solar_hour_;

  // Internal constants
  //
  // Formula [2.18] in book p.36
  // This is denoted I_c in the book.
  static constexpr double kSolarConstant = 1370.0f;

  // This constant is mentioned in book p.41
  static constexpr double kProportionShortwave = 0.15f;

  // This is denoted as σ and is mentioned in book p.35.
  static constexpr double kStefanBoltzmannConstant = 5.67e-8;

  // This is mentioned in book p.48.
  static constexpr double kOffsetAfterSunrise = 1.5f;

  ConstantCaches constant_caches_;

  // TODO: The following may need refactor. It may not need so many private
  // variables. Keeping them just in case some functions may use them. The
  // refactor should start when the first version of the whole plant growth
  // model finishes.

  // Solar azimuth (radians)
  // This is denoted as ϕ in the book.
  double solar_azimuth_;

  // Solar angle from horizontal (radians)
  // This is denoted as β in the book.
  double solar_elevation_;

  // Local solar time for sunrise (hours)
  // This is denoted as t_sr in the book.
  double solar_hour_sunrise_;
  // Local solar time for sunset (hours)
  // This is denoted as t_ss in the book.
  double solar_hour_sunset_;
  // Day length (hours)
  double day_length_;

  // Daily solar irradiance on a horizontal surface
  // They are denoted as I_t_d, I_dr_d, and I_df_d (J m^-2 day^-1) in the book.
  SolarIrradiance daily_solar_irradiance_;

  // Hourly solar irradiance on a horizontal surface
  // They are denoted as I_t, I_dr, and I_df (W m^-2) in the book.
  SolarIrradiance hourly_solar_irradiance_;

  // This is denoted as R_n (W m^-2) in the book.
  double hourly_net_radiation_;

  VaporPressure vapor_pressure_;

  // TODO: In the book, the wind speed is assumed constant. However, we don't
  // have the data for wind speed. Just leave it blank for now.
  double wind_speed_ = 0.0f;

  // Air temperature (°C)
  // This is denoted as T_a in the book.
  double air_temperature_;

  // Updates variables related to geographic data. This should only be called
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
  void UpdateLocalSolarHour(const double solar_hour);
  void UpdateWeather(const Weather &weather);
  void UpdateHourlyNetRadiation(const Weather &weather);

  // These functions below are static, which means they are independent to
  // member variables. We use these functions to calculate member variables in
  // this class.
  static constexpr double DegreeToRadians(const double degree);
  static constexpr double RadiansToDegree(const double radians);

  // Returns solar declination δ (radians) given the day of year.
  static double CalculateSolarDeclination(const int day_of_year);

  // Converts local time (hours) to local solar time (hours) given the day of
  // year, local time (hours), and observer's longitude (radians).
  static double CalculateLocalSolarTime(const int day_of_year,
                                        const double local_hour,
                                        const double longitude);

  // Returns the hour angle τ (radians) given local solar time (hours).
  static double CalculateHourAngle(const double solar_hour);

  // Returns solar angle from horizontal (radians) given solar declination δ
  // (radians), hour angle τ (radians), and observer's latitude λ (radians).
  static double CalculateSolarElevation(const double solar_declination,
                                        const double hour_angle,
                                        const double observer_latitude);

  // Returns solar azimuth (radians) given solar declination δ (radians),
  // observer's latitude λ (radians), solar altitude β (radians), and local
  // solar time (hours).
  static double CalculateSolarAzimuth(const double solar_declination,
                                      const double observer_latitude,
                                      const double solar_altitude,
                                      const double solar_hour);

  // Returns local solar time for sunrise (hours), local solar time for sunset
  // (hours), and day length (hours) given solar declination δ (radians) and
  // observer's latitude λ (radians).
  static DayLengthAndTimesSunriseSunset CalculateDayLength(
      const double solar_declination, const double observer_latitude);

  // Returns daily total solar irradiance (J m^-2 day^-1), daily diffuse
  // irradiance (J m^-2 day^-1), and daily direct irradiance (J m^-2 day^-1)
  // given some pre-calculated constants, climate zone, and day length (hours).
  static SolarIrradiance CalculateDailySolarIrradiance(
      const double I_c_prime, const double a, const double b,
      const Climate::ZoneType climate_zone, const double day_length);

  // Returns hourly solar irradiance (W m^-2), hourly diffuse irradiance (W
  // m^-2), and hourly direct irradiance (W m^-2) given some pre-calculated
  // constants, solar altitude β (radians), daily total solar irradiance (J m^-2
  // day^-1), and local solar time (hours).
  static SolarIrradiance CalculateHourlySolarIrradiance(
      const double solar_altitude, const double I_c_prime, const double a,
      const double b, const double daily_total_solar_irradiance,
      const double solar_hour);

  // Returns hourly net radiation (W m^-2) given hourly total irradiance (W
  // m^-2), air temperature (°C), sunshine hour (hours), and day length (hours).
  static double CalculateHourlyNetRadiation(
      const double hourly_total_irradiance, const double air_temp,
      const double sunshine_hour, const double day_length);

  // Returns saturated vapor pressure (mbar) and actual vapor pressure (mbar)
  // given air temperature (°C) and relative humidity (unit-less).
  static VaporPressure CalculateVaporPressure(const double air_temperature,
                                              const double relative_humidity);

  // Returns air temperature (°C) given solar hour (hours), minimum air
  // temperature (°C), maximum air temperature (°C), solar hour of sunrise
  // (hours), and solar of sunset (hours).
  static double CalculateAirTemperature(double solar_hour,
                                        const double temp_min,
                                        const double temp_max,
                                        const double solar_hour_sunrise,
                                        const double solar_hour_sunset);
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_METEOROLOGY_H_