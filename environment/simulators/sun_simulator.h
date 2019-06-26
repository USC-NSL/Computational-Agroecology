#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_SUN_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_SUN_SIMULATOR_H_

#include "simulator.h"
#include "suninfo.h"

namespace simulator {

class SunSimulator : public Simulator {
 public:
  void SimulateToTime(
      environment::Environment* env,
      const std::chrono::system_clock::time_point& time) override;

 private:
  void GetSunInfo(struct environment::SunInfo* suninfo);

  void GetResult(const int yday, const int hour, const double longitude,
                 const double latitude);
  void GetSolarPosition(const int yday, const int hour, const double longitude,
                        const double latitude);
  void GetDayLength();
  void GetDailyIrradiance();
  void GetDailyDiffuseIrradiance();
  void GetHourlyIrradiance();
  void GetHourlyDiffuseIrradiance();
  double DegreeToRadians(const double degree) const;
  double RadiansToDegree(const double radians) const;

  const double kPI = 3.14159265359;
  const double kPIforDegree = 180.0;
  const double kTropic = -23.45;
  const int kDaysPerYear = 365;
  const int kHoursPerDay = 24;
  const int kDaysLeftPerYear = 10;
  const int kHalfDaysPerDay = 12;
  const int kMinsPerHour = 60;
  const int kSecsPerMin = 60;

  // variables for the GetSolarPosition() function
  int t_d_;
  // the index of the day in the whole year
  double sigma_, gama_standard_meridian_, t_h_, delta_, B_, EoT_, lamda_, beta_,
      alpha_;
  /***
   sigma_ : the solar declination(radians)
   gama_standard_meridian_ : standard meriadian longitude
   t_h_ : local solar time(hours)
   delat_ : the hour angle(radians)
   B_ : just an intermedia variable to calculate the EOT_
   EoT_ : the annual equation of time(minutes)
   lamda_ : the observer's latitude
   beta_ : the solar altitude
   alpha_ : alpha_ + PI is the solar azimuth
  ***/

  // variables for the GetDayLength() function
  double t_ss_, t_sr_, DL_;
  /***
   t_ss_ : the local solar time for sunset
   t_sr_ : the local solar time for sunrise
  ***/

  // varibales for the GetDailyIrradiance() function
  double Ic_, epsilon_0_, Ic_prime_, a_, b_, I_et_d_, b_0_, b_1_, I_t_d_, s_;
  /***
   Ic_ : the radiant flux per unit area
   epsilon_0_ : eccentricity correction factor
   Ic_prime_ : various solar constant Ic_
   a_ : just an intermedia variable to calculate the I_et_d_
   b_ : just an intermedia variable to calculate the I_et_d_
   I_et_d_ : the daily extraterrestrial solar irradiance
   b_0_, b_1_ : the angstrom/empirical cofficients
   I_t_d_ : the daily total irradiance
   s_ : the duration of sunshine hours(hours)
  ***/

  // variables for the GetDailyDiffuseIrradiance() function
  double I_df_d_, I_dr_d_;
  /***
   I_df_d_ : the daily diffuse irradiance
   I_dr_d_ : the daily direct irradiance
  ***/

  // variables for the GetHourlyIrradiance() function
  double psi_, A_prime_, B_prime_, I_t_, I_et_;
  /***
   psi_ : just an intermedia variable to calculate the I_t_
   A_prime_ : just an intermedia variable to calculate the I_t_
   B_prime_ : just an intermedia variable to calculate the I_t_
   I_t_ : the hourly solar irradiance on a horizontal surface (W m^-2)
   I_et_ : the daily solar irradiance on a horizontal surface
  ***/

  // variables for the GetHourlyDiffuseIrradiance() function
  double R_, K_, I_df_, I_dr_;
  /***
   R_ : just an intermedia variable to calculate the I_df_
   K_ : just an intermedia variable to calculate the I_df_
   I_df_ : the hourly diffuse irradiance on a horizontal surface (W m^-2)
   I_dr_ : the hourly direct irradiance on a horizontal surface (W m^-2)
  ***/
};

}  // namespace simulator
#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_SUN_SIMULATOR_H_
