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
	GetResult(tm->tm_year,
      tm->tm_mon,
      tm->tm_mday,
      tm->tm_hour, 
      (env->config_.location.longitude_left + env->config_.location.longitude_right) / 2.0,
      (env->config_.location.latitude_bottom + env->config_.location.latitude_top) / 2.0
    );
    env->sunInfo_ = GetSunInfo();
  }

  void SunSimulator::GetResult(int _year, int _month, int _day, int _hour, double _longitude, double _latitude) {
	FirstStep(_year, _month, _day, _hour, _longitude, _latitude);
    SecondStep();
    ThirdStep();
    FourthStep();
    FifthStep();
    SixthStep();
  }

  struct environment::SunInfo SunSimulator::GetSunInfo() const {
    struct environment::SunInfo t;
    t._sunAzimuth = alpha_ + kPI;
    t._solarAltitude = kPI / 2.0 - beta_;
    t._hourlyIrradiance = I_t_;
    return t;
  }

  double SunSimulator::DegreeToRadians(double _degree) const {
    return _degree * kPI / 180.0;
  }

  double SunSimulator::RadiansToDegree(double _radians) const {
    return _radians / kPI * 180.0;
  }

  void SunSimulator::FirstStep(int _year, int _month, int _day, int _hour, double _longitude, double _latitude) {
    is_leapYear = (((_year % 4 == 0) && (_year % 100 != 0)) || _year % 400 == 0);
    t_d_ = _day;
    switch (_month) {
    case 12:t_d_ += 30;
    case 11:t_d_ += 31;
    case 10:t_d_ += 30;
    case 9:t_d_ += 31;
    case 8:t_d_ += 31;
    case 7:t_d_ += 30;
    case 6:t_d_ += 31;
    case 5:t_d_ += 30;
    case 4:t_d_ += 31;
    case 3:t_d_ += 28 + is_leapYear;
    case 2:t_d_ += 31;
    case 1:break;
    default:break;
    }
    sigma_ = -23.45 * kPI / 180.0 * cos(2.0 * kPI * (t_d_ + 10) / (365 + is_leapYear));
    gama_standard_meridian_ = ((int)(_longitude / kPI * 12)) * kPI / 12;
    B_ = 2.0 * kPI * (t_d_ - 81) / (364 + is_leapYear);
    EoT_ = 9.87 * sin(2 * B_) - 7.53 * cos(B_) - 1.5 * sin(B_);
    th_ = _hour + (gama_standard_meridian_ - DegreeToRadians(_longitude)) / kPI * 12 + EoT_ / 60;
    delta_ = kPI / 12.0 * (th_ - 12);
    lamda_ = DegreeToRadians(_latitude);
    beta_ = asin(sin(sigma_) * sin(lamda_) + cos(sigma_) * cos(lamda_) * cos(delta_));
    alpha_ = acos((sin(lamda_) * sin(beta_) - sin(sigma_)) / (cos(lamda_) * cos(beta_)));
    if (_hour < 12) {
      alpha_ *= -1.0;
    }
  }

  void SunSimulator::SecondStep() {
    t_ss_ = 12 + 12 / kPI * acos(-(sin(sigma_) * sin(lamda_)) / (cos(sigma_) * cos(lamda_)));
    t_sr_ = 24 - t_ss_;
    DL_ = 2 * (t_ss_ - 12);
  }

  void SunSimulator::ThirdStep() {
    Ic_ = 1370.0;
    epsilon_0_ = 1.0 + 0.033 * cos(2.0 * kPI * (t_d_ - 10) / (365 + is_leapYear));
    Ic_prime_ = Ic_ * epsilon_0_;
    a_ = sin(lamda_) * sin(sigma_);
    b_ = cos(lamda_) * cos(sigma_);
    I_et_d_ = 3600.0 * Ic_prime_ * 24 / kPI * (a_ * acos(-a_ / b_) + b_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
    {b_0_ = 0.18, b_1_ = 0.55; }//Cold or temperate
    {b_0_ = 0.25, b_1_ = 0.45; }//Dry tropical
    {b_0_ = 0.29, b_1_ = 0.42; }//Humid tropical
    {s_ = DL_; } // have all sunshine in the day time
    I_t_d_ = I_et_d_ * (b_0_ + b_1_ * s_ / DL_);
  }

  void SunSimulator::FourthStep() {
    if (I_t_d_ / I_et_d_ < 0.07)
      I_df_d_ = I_t_d_;
    else if (I_t_d_ / I_et_d_ < 0.35)
      I_df_d_ = I_t_d_ * (1 - 2.3 * (I_t_d_ / I_et_d_ - 0.07) * (I_t_d_ / I_et_d_ - 0.07));
    else if (I_t_d_ / I_et_d_ < 0.75)
      I_df_d_ = I_t_d_ * (1.33 - 1.46 * I_t_d_ / I_et_d_);
    else
      I_df_d_ = I_t_d_ * 0.23;
    I_dr_d_ = I_t_d_ - I_df_d_;
  }

  void SunSimulator::FifthStep() {
    psi_ = kPI * I_t_d_ / 86400 / (a_ * acos(-a_ / b_) + b_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
    A_prime_ = -b_ * psi_;
    B_prime_ = a_ * psi_;
    I_t_ = 86400 / kPI * (B_prime_ * acos(-a_ / b_) - A_prime_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
    I_et_ = Ic_prime_ * sin(beta_);
  }

  void SunSimulator::SixthStep() {
    R_ = (0.847 - 1.61 * sin(beta_) + 1.04 * sin(beta_) * sin(beta_));
    K_ = (1.47 - R_) / 1.66;
    if (I_t_ / I_et_ <= 0.22)
      I_df_ = I_t_;
    else if (I_t_ / I_et_ <= 0.35)
      I_df_ = I_t_ * (1 - 6.4 * (I_t_ / I_et_ - 0.22) * (I_t_ / I_et_ - 0.22));
    else if (I_t_ / I_et_ <= K_)
      I_df_ = I_t_ * (1.47 - 1.66 * I_t_ / I_et_);
    else
      I_df_ = I_t_ * R_;
    I_dr_ = I_t_ - I_df_;
  }
}