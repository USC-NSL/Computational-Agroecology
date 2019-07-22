#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUNINFO_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUNINFO_H_

#include <chrono>

#include "location.h"

namespace environment {

const double kPI = 3.14159265359;
const double kPIforDegree = 180.0;
const double kTropic = -23.45;
const int kDaysPerYear = 365;
const int kHoursPerDay = 24;
const int kDaysLeftPerYear = 10;
const int kHoursHalfDay = 12;
const int kMinsPerHour = 60;
const int kSecsPerMin = 60;

class SunInfo {
 public:
  const double &sun_azimuth() const { return sun_azimuth_; }
  const double &solar_altitude() const { return solar_altitude_; }
  const double &hourly_irradiance() const { return hourly_irradiance_; }

  void SimulateToTime(const std::chrono::system_clock::time_point &time,
                      const Location &location);

 private:
  double sun_azimuth_;
  double solar_altitude_;
  double hourly_irradiance_;

  void GetResult(const int yday, const int hour, const double longitude,
                 const double latitude);
  // yday means the index of the day in whole year, see the definition of struct
  // tm
  void GetSolarPosition(const int yday, const int hour, const double longitude,
                        const double latitude);
  // this function is used to calculate the solor position by day, hour,
  // longitude, and latitude.
  void GetDayLength();
  // this function is used to calculate the length of the certain day(yday)
  void GetDailyIrradiance();
  // this function is used to calculate the daily solar irradiance for the
  // certain day(yday)
  void GetDailyDiffuseIrradiance();
  // this function is used to calculate the diffuse and direct daily solar
  // irradiance for the certain day(yday)
  void GetHourlyIrradiance();
  // this function is used to calculate the hourly solar irradiance for the
  // day(yday)
  void GetHourlyDiffuseIrradiance();
  // this function is used to calculate the diffuse and direct hourly solar
  // irradiance for the certain day(yday)
  double DegreeToRadians(const double degree) const;
  double RadiansToDegree(const double radians) const;

  // variables for the GetSolarPosition() function
  int t_d_;                        // the index of the day in the whole year
  double sigma_;                   // the solar declination(radians)
  double gama_standard_meridian_;  // standard meriadian longitude
  double t_h_;                     // local solar time(hours)
  double delta_;                   // the hour angle(radians)
  double EoT_;                     // the annual equation of time(minutes)
  double lamda_;                   // the observer's latitude
  double beta_;                    // the solar altitude
  double alpha_;                   // alpha_ + PI is the solar azimuth

  // variables for the GetDayLength() function
  double t_ss_;  // the local solar time for sunset
  double t_sr_;  // the local solar time for sunrise
  double DL_;    // the length of the certain day(yday)

  // varibales for the GetDailyIrradiance() function
  double Ic_;         // the radiant flux per unit area
  double epsilon_0_;  // eccentricity correction factor
  double Ic_prime_;   // various solar constant Ic_
  double I_et_d_;     // the daily extraterrestrial solar irradiance
  double b_0_;        // the angstrom/empirical cofficients
  double b_1_;        // the angstrom/empirical cofficients
  double I_t_d_;      // the daily total irradiance
  double s_;          // the duration of sunshine hours(hours)

  // variables for the GetDailyDiffuseIrradiance() function
  double I_df_d_;  // the daily diffuse irradiance
  double I_dr_d_;  // the daily direct irradiance

  // variables for the GetHourlyIrradiance() function
  double I_t_;   // the hourly solar irradiance on a horizontal surface (W m^-2)
  double I_et_;  // the daily solar irradiance on a horizontal surface

  // variables for the GetHourlyDiffuseIrradiance() function
  double
      I_df_;  // the hourly diffuse irradiance on a horizontal surface (W m^-2)
  double
      I_dr_;  // the hourly direct irradiance on a horizontal surface (W m^-2)
};

}  // namespace environment
#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUNINFO_H_