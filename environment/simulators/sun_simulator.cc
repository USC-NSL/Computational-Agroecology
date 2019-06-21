#include "sun_simulator.h"

#include <cmath>
#include <iostream>
#include <cassert>
#include <ctime>

#include "../environment.h" // if you make sure that relative path is not needed, you can remove it, but on Windos I need it.


namespace simulator {

  SunSimulator::SunSimulator() {}

  void SunSimulator::SimulateToTime(
    environment::Environment* env,
    const std::chrono::system_clock::time_point& time) {
    time_t tt = std::chrono::system_clock::to_time_t(time); 
    struct tm* tm = localtime(&tt);
    getResult(tm->tm_year,
      tm->tm_mon,
      tm->tm_mday,
      tm->tm_hour, 
      (env->config_.location.longitude_left + env->config_.location.longitude_right) / 2.0,
      (env->config_.location.latitude_bottom + env->config_.location.latitude_top) / 2.0
    );
    env->sunInfo_ = get_sunInfo();
  }

  void SunSimulator::getResult(int _year, int _month, int _day, int _hour, double _longitude, double _latitude) {
    first_step(_year, _month, _day, _hour, _longitude, _latitude);
    second_step();
    third_step();
    fourth_step();
    fifth_step();
    sixth_step();
  }

  struct environment::SunInfo SunSimulator::get_sunInfo() const {
    struct environment::SunInfo t;
    t._sunAzimuth = alpha + PI;
    t._solarAltitude = PI / 2.0 - beta;
    t._hourlyIrradiance = I_t;
    return t;
  }

  double SunSimulator::_degree_to_radians(double _degree) const {
    return _degree * PI / 180.0;
  }

  double SunSimulator::_radians_to_degree(double _radians) const {
    return _radians / PI * 180.0;
  }

  void SunSimulator::first_step(int _year, int _month, int _day, int _hour, double _longitude, double _latitude) {
    is_leapYear = (((_year % 4 == 0) && (_year % 100 != 0)) || _year % 400 == 0);
    t_d = _day;
    switch (_month) {
    case 12:t_d += 30;
    case 11:t_d += 31;
    case 10:t_d += 30;
    case 9:t_d += 31;
    case 8:t_d += 31;
    case 7:t_d += 30;
    case 6:t_d += 31;
    case 5:t_d += 30;
    case 4:t_d += 31;
    case 3:t_d += 28 + is_leapYear;
    case 2:t_d += 31;
    case 1:break;
    default:break;
    }
    sigma = -23.45 * PI / 180.0 * cos(2.0 * PI * (t_d + 10) / (365 + is_leapYear));
    gama_standard_meridian = ((int)(_longitude / PI * 12)) * PI / 12;
    B = 2.0 * PI * (t_d - 81) / (364 + is_leapYear);
    EoT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
    th = _hour + (gama_standard_meridian - _degree_to_radians(_longitude)) / PI * 12 + EoT / 60;
    delta = PI / 12.0 * (th - 12);
    lamda = _degree_to_radians(_latitude);
    beta = asin(sin(sigma) * sin(lamda) + cos(sigma) * cos(lamda) * cos(delta));
    alpha = acos((sin(lamda) * sin(beta) - sin(sigma)) / (cos(lamda) * cos(beta)));
    if (_hour < 12) {
      alpha *= -1.0;
    }
  }

  void SunSimulator::second_step() {
    t_ss = 12 + 12 / PI * acos(-(sin(sigma) * sin(lamda)) / (cos(sigma) * cos(lamda)));
    t_sr = 24 - t_ss;
    DL = 2 * (t_ss - 12);
  }

  void SunSimulator::third_step() {
    Ic = 1370.0;
    epsilon_0 = 1.0 + 0.033 * cos(2.0 * PI * (t_d - 10) / (365 + is_leapYear));
    Ic_prime = Ic * epsilon_0;
    a = sin(lamda) * sin(sigma);
    b = cos(lamda) * cos(sigma);
    I_et_d = 3600.0 * Ic_prime * 24 / PI * (a * acos(-a / b) + b * sqrt(1 - (a / b) * (a / b)));
    {b_0 = 0.18, b_1 = 0.55; }//Cold or temperate
    {b_0 = 0.25, b_1 = 0.45; }//Dry tropical
    {b_0 = 0.29, b_1 = 0.42; }//Humid tropical
    {s = DL; } // have all sunshine in the day time
    I_t_d = I_et_d * (b_0 + b_1 * s / DL);
  }

  void SunSimulator::fourth_step() {
    if (I_t_d / I_et_d < 0.07)
      I_df_d = I_t_d;
    else if (I_t_d / I_et_d < 0.35)
      I_df_d = I_t_d * (1 - 2.3 * (I_t_d / I_et_d - 0.07) * (I_t_d / I_et_d - 0.07));
    else if (I_t_d / I_et_d < 0.75)
      I_df_d = I_t_d * (1.33 - 1.46 * I_t_d / I_et_d);
    else
      I_df_d = I_t_d * 0.23;
    I_dr_d = I_t_d - I_df_d;
  }

  void SunSimulator::fifth_step() {
    psi = PI * I_t_d / 86400 / (a * acos(-a / b) + b * sqrt(1 - (a / b) * (a / b)));
    A_prime = -b * psi;
    B_prime = a * psi;
    I_t = 86400 / PI * (B_prime * acos(-a / b) - A_prime * sqrt(1 - (a / b) * (a / b)));
    I_et = Ic_prime * sin(beta);
  }

  void SunSimulator::sixth_step() {
    R = (0.847 - 1.61 * sin(beta) + 1.04 * sin(beta) * sin(beta));
    K = (1.47 - R) / 1.66;
    if (I_t / I_et <= 0.22)
      I_df = I_t;
    else if (I_t / I_et <= 0.35)
      I_df = I_t * (1 - 6.4 * (I_t / I_et - 0.22) * (I_t / I_et - 0.22));
    else if (I_t / I_et <= K)
      I_df = I_t * (1.47 - 1.66 * I_t / I_et);
    else
      I_df = I_t * R;
    I_dr = I_t - I_df;
  }
}