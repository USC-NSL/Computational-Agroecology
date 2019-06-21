#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H

#include "simulator.h"
namespace environment {
  struct SunInfo {
    double _sunAzimuth, _solarAltitude, _hourlyIrradiance;
  };
}

namespace simulator {

  class SunSimulator : public Simulator {
  public:
    SunSimulator();
    void SimulateToTime(
      environment::Environment* env,
      const std::chrono::system_clock::time_point& time) override;
  //the following function just for test
  struct environment::SunInfo get_sunInfo_for_test(int _year, int _month, int _day, int _hour, double _longitude, double _latitude) {
    getResult(_year, _month, _day, _hour, _longitude, _latitude);
    return get_sunInfo();
  }
  private:
    struct environment::SunInfo get_sunInfo() const;

    void getResult(int _year, int _month, int _day, int _hour, double _longitude, double _latitude);
    void first_step(int _year, int _month, int _day, int _hour, double _longitude, double _latitude);
    void second_step();
    void third_step();
    void fourth_step();
    void fifth_step();
    void sixth_step();
    double _degree_to_radians(double _degree) const;
    double _radians_to_degree(double _radians) const;

    const double PI = 3.14159265359;

    //variables for the first step
    bool is_leapYear;
    int t_d;
    double sigma, gama_standard_meridian, th, delta, B, EoT, solar_angel_radians, lamda, beta, alpha;

    //variables for the second step
    double t_ss, t_sr, DL;

    //varibales for the third step
    double Ic, epsilon_0, Ic_prime, a, b, I_et_d, b_0, b_1, I_t_d, s;

    //variables for the fourth step
    double I_df_d, I_dr_d;

    //variables for the fifth step
    double psi, A_prime, B_prime, I_t, I_et;

    //variables for the sixth step
    double R, K, I_df, I_dr;
  };

} // namespace simulator
#endif COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H
