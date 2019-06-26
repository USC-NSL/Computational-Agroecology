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
  GetResult(tm->tm_yday + 1, tm->tm_hour,
            (env->config_.location.longitude_left +
             env->config_.location.longitude_right) /
                2.0,
            (env->config_.location.latitude_bottom +
             env->config_.location.latitude_top) /
                2.0);
  GetSunInfo(&env->sun_info_);
}

void SunSimulator::GetResult(const int yday, const int hour,
                             const double longitude, const double latitude) {
  GetSolarPosition(yday, hour, longitude, latitude);
  GetDayLength();
  GetDailyIrradiance();
  GetDailyDiffuseIrradiance();
  GetHourlyIrradiance();
  GetHourlyDiffuseIrradiance();
}

void SunSimulator::GetSunInfo(struct environment::SunInfo* suninfo) {
  suninfo->SunAzimuth = alpha_ + kPI;
  suninfo->SolarAltitude = kPI / 2.0 - beta_;
  suninfo->HourlyIrradiance = I_t_;
}

double SunSimulator::DegreeToRadians(const double degree) const {
  return degree * kPI / kPIforDegree;
}

double SunSimulator::RadiansToDegree(const double radians) const {
  return radians / kPI * kPIforDegree;
}

void SunSimulator::GetSolarPosition(const int yday, const int hour,
                             const double longitude, const double latitude) {
  t_d_ = yday;
  sigma_ = -kTropic * kPI / kPIforDegree *
           cos(2.0 * kPI * (t_d_ + kDaysLeftPerYear) / kDaysPerYear);
  gama_standard_meridian_ = ((int)(longitude / kPI * kHalfDaysPerDay)) * kPI / kHalfDaysPerDay;
  B_ = 2.0 * kPI * (t_d_ - 81) / (kDaysPerYear - 1);
  //Introduction to mathematical modeling of crop growth p.29 formulat [2.6]
  EoT_ = 9.87 * sin(2 * B_) - 7.53 * cos(B_) - 1.5 * sin(B_);
  //Introduction to mathematical modeling of crop growth p.29 formulat [2.6]
  t_h_ = hour +
        (gama_standard_meridian_ - DegreeToRadians(longitude)) / kPI * kHalfDaysPerDay +
        EoT_ / kMinsPerHour;
  delta_ = kPI / kHalfDaysPerDay * (t_h_ - kHalfDaysPerDay);
  lamda_ = DegreeToRadians(latitude);
  beta_ =
      asin(sin(sigma_) * sin(lamda_) + cos(sigma_) * cos(lamda_) * cos(delta_));
  alpha_ = acos((sin(lamda_) * sin(beta_) - sin(sigma_)) /
                (cos(lamda_) * cos(beta_)));
  if (hour < kHalfDaysPerDay) {
    alpha_ *= -1.0;
  }
}

void SunSimulator::GetDayLength() {
  t_ss_ =
      kHalfDaysPerDay + kHalfDaysPerDay / kPI *
               acos(-(sin(sigma_) * sin(lamda_)) / (cos(sigma_) * cos(lamda_)));
  t_sr_ = kHoursPerDay - t_ss_;
  DL_ = 2 * (t_ss_ - kHalfDaysPerDay);
}

void SunSimulator::GetDailyIrradiance() {
  Ic_ = 1370.0; 
  //Introduction to mathematical modeling of crop growth p.36 formulat [2.18]
  epsilon_0_ =
      1.0 + 0.033 * cos(2.0 * kPI * (t_d_ - kDaysLeftPerYear) / kDaysPerYear);
  //Introduction to mathematical modeling of crop growth p.37 formulat [2.21]
  Ic_prime_ = Ic_ * epsilon_0_;
  a_ = sin(lamda_) * sin(sigma_);
  b_ = cos(lamda_) * cos(sigma_);
  I_et_d_ = kSecsPerMin * kMinsPerHour * Ic_prime_ * kHoursPerDay / kPI *
            (a_ * acos(-a_ / b_) + b_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
  { b_0_ = 0.18, b_1_ = 0.55; }  // Cold or temperate
  { b_0_ = 0.25, b_1_ = 0.45; }  // Dry tropical
  { b_0_ = 0.29, b_1_ = 0.42; }  // Humid tropical
  { s_ = DL_; }                  // have all sunshine in the day time
  I_t_d_ = I_et_d_ * (b_0_ + b_1_ * s_ / DL_);
}

void SunSimulator::GetDailyDiffuseIrradiance() {
  //Introduction to mathematical modeling of crop growth p.38 formulat [2.25]
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

void SunSimulator::GetHourlyIrradiance() {
  psi_ = kPI * I_t_d_ / kSecsPerMin / kMinsPerHour / kHoursPerDay /
         (a_ * acos(-a_ / b_) + b_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
  A_prime_ = -b_ * psi_;
  B_prime_ = a_ * psi_;
  I_t_ =
	  kSecsPerMin * kMinsPerHour * kHoursPerDay / kPI *
      (B_prime_ * acos(-a_ / b_) - A_prime_ * sqrt(1 - (a_ / b_) * (a_ / b_)));
  I_et_ = Ic_prime_ * sin(beta_);
}

void SunSimulator::GetHourlyDiffuseIrradiance() {
  //Introduction to mathematical modeling of crop growth p.40 formulat [2.31]
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
