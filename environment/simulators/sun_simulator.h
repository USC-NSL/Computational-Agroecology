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

  void GetResult(int yday, int hour, double longitude,
                 double latitude);
  void GetSolarPosition(int yday, int hour, double longitude,
                 double latitude);
  void GetDayLength();
  void GetDailyIrradiance();
  void GetDailyDiffuseIrradiance();
  void GetHourlyIrradiance();
  void GetHourlyDiffuseIrradiance();
  double DegreeToRadians(double degree) const;
  double RadiansToDegree(double radians) const;

  const double kPI = 3.14159265359;
  const double kPIforDegree = 180.0;
  const double kTropic = -23.45;
  const int kDaysPerYear = 365;
  const int kHoursPerDay = 24;
  const int kDaysLeftPerYear = 10;
  const int kHalfDaysPerDay = 12;
  const int kMinsPerHour = 60;
  const int kSecsPerMin = 60;
  // variables for the first step

  int t_d_;
  double sigma_, gama_standard_meridian_, t_h_, delta_, B_, EoT_, lamda_, beta_,
      alpha_;

  // variables for the second step
  double t_ss_, t_sr_, DL_;

  // varibales for the third step
  double Ic_, epsilon_0_, Ic_prime_, a_, b_, I_et_d_, b_0_, b_1_, I_t_d_, s_;

  // variables for the fourth step
  double I_df_d_, I_dr_d_;

  // variables for the fifth step
  double psi_, A_prime_, B_prime_, I_t_, I_et_;

  // variables for the sixth step
  double R_, K_, I_df_, I_dr_;
};

}  // namespace simulator
#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_SUN_SIMULATOR_H_
