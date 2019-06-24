#ifndef COMPUTATIONAL_AGROECOLOGY_SIMULATORS_SUN_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_SIMULATORS_SUN_SIMULATOR_H_

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

  void GetResult(int year, int month, int day, int hour, double longitude,
                 double latitude);
  void FirstStep(int year, int month, int day, int hour, double longitude,
                 double latitude);
  void SecondStep();
  void ThirdStep();
  void FourthStep();
  void FifthStep();
  void SixthStep();
  double DegreeToRadians(double degree) const;
  double RadiansToDegree(double radians) const;

  const double kPI = 3.14159265359;

  // variables for the first step
  bool is_leapYear_;
  int t_d_;
  double sigma_, gama_standard_meridian_, th_, delta_, B_, EoT_, lamda_, beta_,
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
#endif  // COMPUTATIONAL_AGROECOLOGY_SIMULATORS_SUN_SIMULATOR_H_
