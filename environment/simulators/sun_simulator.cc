#include "sun_simulator.h"

#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>

#include "environment.h"  // if you make sure that relative path is not needed, you can remove it, but on Windos I need it.

namespace simulator {

void SunSimulator::SimulateToTime(
    environment::Environment* env,
    const std::chrono::system_clock::time_point& time) {
  time_t tt = std::chrono::system_clock::to_time_t(time);
  struct tm* tm = localtime(&tt);
  GetResult(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
            (env->config_.location.longitude_left +
             env->config_.location.longitude_right) /
                2.0,
            (env->config_.location.latitude_bottom +
             env->config_.location.latitude_top) /
                2.0);
  GetSunInfo(&env->sun_info_);
}

void SunSimulator::GetResult(int year, int month, int day, int hour,
                             double longitude, double latitude) {
  FirstStep(year, month, day, hour, longitude, latitude);
  SecondStep();
  ThirdStep();
  FourthStep();
  FifthStep();
  SixthStep();
}

void SunSimulator::GetSunInfo(struct environment::SunInfo* suninfo) {
  suninfo->SunAzimuth = alpha_ + kPI;
  suninfo->SolarAltitude = kPI / 2.0 - beta_;
  suninfo->HourlyIrradiance = I_t_;
}

double SunSimulator::DegreeToRadians(double degree) const {
  return degree * kPI / 180.0;
}

double SunSimulator::RadiansToDegree(double radians) const {
  return radians / kPI * 180.0;
}

void SunSimulator::FirstStep(int year, int month, int day, int hour,
                             double longitude, double latitude) {
  is_leapYear_ = (((year % 4 == 0) && (year % 100 != 0)) || year % 400 == 0);
  t_d_ = day;
  switch (month) {
    case 12:
      t_d_ += 30;
    case 11:
      t_d_ += 31;
    case 10:
      t_d_ += 30;
    case 9:
      t_d_ += 31;
    case 8:
      t_d_ += 31;
    case 7:
      t_d_ += 30;
    case 6:
      t_d_ += 31;
    case 5:
      t_d_ += 30;
    case 4:
      t_d_ += 31;
    case 3:
      t_d_ += 28 + is_leapYear_;
    case 2:
      t_d_ += 31;
    case 1:
      break;
    default:
      break;
  }
  sigma_ = -23.45 * kPI / 180.0 *
           cos(2.0 * kPI * (t_d_ + 10) / (365 + is_leapYear_));
  gama_standard_meridian_ = ((int)(longitude / kPI * 12)) * kPI / 12;
  B_ = 2.0 * kPI * (t_d_ - 81) / (364 + is_leapYear_);
  EoT_ = 9.87 * sin(2 * B_) - 7.53 * cos(B_) - 1.5 * sin(B_);
  th_ = hour +
        (gama_standard_meridian_ - DegreeToRadians(longitude)) / kPI * 12 +
        EoT_ / 60;
  delta_ = kPI / 12.0 * (th_ - 12);
  lamda_ = DegreeToRadians(latitude);
  beta_ =
      asin(sin(sigma_) * sin(lamda_) + cos(sigma_) * cos(lamda_) * cos(delta_));
  alpha_ = acos((sin(lamda_) * sin(beta_) - sin(sigma_)) /
                (cos(lamda_) * cos(beta_)));
  if (hour < 12) {
    alpha_ *= -1.0;
  }
}

void SunSimulator::SecondStep() {
  t_ss_ =
      12 + 12 / kPI *
               acos(-(sin(sigma_) * sin(lamda_)) / (cos(sigma_) * cos(lamda_)));
  t_sr_ = 24 - t_ss_;
  DL_ = 2 * (t_ss_ - 12);
}

void SunSimulator::ThirdStep() {
  Ic_ = 1370.0;
  epsilon_0_ =
      1.0 + 0.033 * cos(2.0 * kPI * (t_d_ - 10) / (365 + is_leapYear_));
  Ic_prime_ = Ic_ * epsilon_0_;
  a_ = sin(lamda_) * sin(sigma_);
  b_ = cos(lamda_) * cos(sigma_);
  I_et_d_ = 3600.0 * Ic_prime_ * 24 / kPI *
            (a_ * acos(-a_ / b_) + b_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
  { b_0_ = 0.18, b_1_ = 0.55; }  // Cold or temperate
  { b_0_ = 0.25, b_1_ = 0.45; }  // Dry tropical
  { b_0_ = 0.29, b_1_ = 0.42; }  // Humid tropical
  { s_ = DL_; }                  // have all sunshine in the day time
  I_t_d_ = I_et_d_ * (b_0_ + b_1_ * s_ / DL_);
}

void SunSimulator::FourthStep() {
  if (I_t_d_ / I_et_d_ < 0.07)
    I_df_d_ = I_t_d_;
  else if (I_t_d_ / I_et_d_ < 0.35)
    I_df_d_ = I_t_d_ *
              (1 - 2.3 * (I_t_d_ / I_et_d_ - 0.07) * (I_t_d_ / I_et_d_ - 0.07));
  else if (I_t_d_ / I_et_d_ < 0.75)
    I_df_d_ = I_t_d_ * (1.33 - 1.46 * I_t_d_ / I_et_d_);
  else
    I_df_d_ = I_t_d_ * 0.23;
  I_dr_d_ = I_t_d_ - I_df_d_;
}

void SunSimulator::FifthStep() {
  psi_ = kPI * I_t_d_ / 86400 /
         (a_ * acos(-a_ / b_) + b_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
  A_prime_ = -b_ * psi_;
  B_prime_ = a_ * psi_;
  I_t_ =
      86400 / kPI *
      (B_prime_ * acos(-a_ / b_) - A_prime_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
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
}  // namespace simulator
