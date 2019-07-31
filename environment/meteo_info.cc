#include "meteo_info.h"

#include <cassert>
#include <cmath>
#include <ctime>

#include <iostream>

namespace environment {

MeteoInfo::MeteoInfo(const std::chrono::system_clock::time_point &time,
                     const Location &location,
                     const Climate::ZoneType climate_zone,
                     const Weather &weather) {
  SimulateToTime(time, location, climate_zone, weather);
}

void MeteoInfo::SimulateToTime(
    const std::chrono::system_clock::time_point &time, const Location &location,
    const Climate::ZoneType climate_zone, const Weather &weather) {
  time_t tt = std::chrono::system_clock::to_time_t(time);
  struct tm *tm = localtime(&tt);
  double longitude = (location.longitude_left + location.longitude_right) / 2.0;
  double latitude = (location.latitude_bottom + location.latitude_top) / 2.0;

  Update(tm->tm_yday + 1, tm->tm_hour, longitude, latitude, climate_zone,
         weather.temperature.min, weather.temperature.max);
}

const double MeteoInfo::DegreeToRadians(const double degree) {
  return degree * kPI / kPIforDegree;
}

const double MeteoInfo::RadiansToDegree(const double radians) {
  return radians / kPI * kPIforDegree;
}

void MeteoInfo::Update(const int t_d, const int hour, const double longitude,
                       const double latitude,
                       const Climate::ZoneType climate_zone,
                       const double temp_min, const double temp_max) {
  // Local solar time (hours)
  double local_solar_time = CalculateLocalSolarTime(t_d, hour, longitude);

  // Calculate shared constants, delta δ, tau τ, and labmda λ
  // Solar declination δ (radians)
  double delta = CalculateSolarDeclination(t_d);
  // The hour angle τ (radians)
  double tau = CalculateHourAngle(local_solar_time);
  // Observer's latitude λ
  double lambda = DegreeToRadians(latitude);

  // Update Solar Altitude
  solar_altitude_ = CalculateSolarAltitude(delta, tau, lambda);
  // Update Solar Azimuth
  solar_azimuth_ =
      CalculateSolarAzimuth(delta, lambda, solar_altitude_, local_solar_time);

  // Update day length
  std::tie(t_sr_, t_ss_, day_length_) = CalculateDayLength(delta, tau, lambda);

  // Prepare some shared constants for solar irradiance
  // Formula [2.18] in book p.36
  // Solar constant
  const double I_c = 1370.0f;

  // Formula [2.21] in book p.37
  // ε_0 is approximated by the following equation
  // ε_0 ≈ 1 + 0.033 * cos(2 * π * (t_d - 10) / 365)
  double epsilon_0 =
      1 + 0.033 * cos(k2PI * (t_d - kDaysLeftPerYear) / kDaysPerYear);

  // Formula [2.19] in book p.37
  // I_c_prime = ε_0 * I_c a = sin(λ) * sin(δ) b = cos(λ) * cos(δ)
  double I_c_prime = epsilon_0 * I_c;

  // to solve integral sin(β), we need the following a and b
  double a = sin(lambda) * sin(delta);
  double b = cos(lambda) * cos(delta);

  // Update daily irradiance
  std::tie(I_t_d_, I_df_d_, I_dr_d_) =
      CalculateDailySolarIrradiance(I_c_prime, a, b, climate_zone, day_length_);

  // Update hourly irradiance
  std::tie(I_t_, I_df_, I_dr_) = CalculateHourlySolarIrradiance(
      solar_altitude_, I_c_prime, a, b, I_t_d_, local_solar_time);

  // Update air temperature
  T_a_ = CalculateAirTemperature(local_solar_time, temp_min, temp_max, t_sr_,
                                 t_ss_);

  // Update saturated vapor pressure
  e_s_T_a_ = CalculateSaturatedVaporPressure(T_a_);
}

double MeteoInfo::CalculateSolarDeclination(const int t_d) {
  // Formula [2.1], [2.2] in book p.25
  // y = A cos (2 * π * x / P) + B
  // A: Amplitude (highest peak and lowest vallet for the cycle)
  // P: Time period of the cycle (time after which the cycle repeats itself)
  // B: The height of the vertical offset

  // In this function:
  // δ = A cos (2 * π * (t_d + 10) / P) + B
  // A: -23.45 degrees in radian
  // P: 365 days
  // B: 0
  // t_d: the day of the year
  // +10: to shift Dec 21 to Dec 31 (On Dec 21, we would get the minimum)

  const double A = DegreeToRadians(-kTropic);
  return A * cos(k2PI * (t_d + kDaysLeftPerYear) / kDaysPerYear);
}

double MeteoInfo::CalculateLocalSolarTime(const int t_d, const int hour,
                                          const double longitude) {
  // Formula [2.4], [2,5], [2.6] in book p.27
  // t_h = t + ((γ_sm - γ) / (π / 12)) + (EoT / 60)
  // t: local time (hours)
  // γ_sm (Standard Meridian logitude)
  //   = int(γ / (π / 12)) * (π / 12)
  // γ: observer's longitude
  // EoT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B)
  // B = 2 * π * (t_d -81) / 364

  // Standard Meridian logitude
  double gamma_sm = std::floor(longitude / kPiDividedBy12) * kPiDividedBy12;
  double B = k2PI * (t_d - 81) / (kDaysPerYear - 1);
  double EoT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
  return hour + ((gamma_sm - DegreeToRadians(longitude)) / kPiDividedBy12) +
         (EoT / kMinsPerHour);
}

double MeteoInfo::CalculateHourAngle(const double t_h) {
  // Formula [2.3] in book p.27
  // τ = (π / 12) * (t_h - 12)
  return kPiDividedBy12 * (t_h - kHoursHalfDay);
}

double MeteoInfo::CalculateSolarAltitude(const double delta, const double tau,
                                         const double lambda) {
  // Formula [2.8] in book p.30
  // β: Solar's altitude with respect to the observer
  // sin(β) = sin(δ) * sin(λ) + cos(δ) * cos(λ) * cos(τ)
  double sin_beta =
      sin(delta) * sin(lambda) + cos(delta) * cos(lambda) * cos(tau);
  return asin(sin_beta);
}

double MeteoInfo::CalculateSolarAzimuth(const double delta, const double lambda,
                                        const double beta, const double t_h) {
  // Formula [2.9], [2.10] in book p.30, p.31
  // β: Solar's altitude with respect to the observer
  // ϕ: Solar Azimuth
  // cos(α) = (sin(λ) * sin(β) - sin(δ)) / (cos(λ) * cos(β))
  // α = ϕ - π
  //   = ±acos((sin(λ) * sin(β) - sin(δ)) / (cos(λ) * cos(β)))
  double cos_alpha =
      (sin(lambda) * sin(beta) - sin(delta)) / (cos(lambda) * cos(beta));
  cos_alpha = std::min(cos_alpha, 1.0);  // ensure cos(α) <= 1.0
  double alpha = acos(cos_alpha);

  // if before solar noon, alpha < 0
  if (t_h < kHoursHalfDay) {
    alpha = -alpha;
  }

  return kPI + alpha;
}

std::tuple<double, double, double> MeteoInfo::CalculateDayLength(
    const double delta, const double tau, const double lambda) {
  // Formula [2.11] in book p.31
  // t_ss = 12 + (12 / π) * acos(-(sin(δ) * sin(λ) / (cos(δ) * cos(λ))))

  // tmp = -((sin(δ) * sin(λ)) / (cos(δ) * cos(λ)))
  double tmp = -((sin(delta) * sin(lambda)) / (cos(delta) * cos(lambda)));
  double t_ss = kHoursHalfDay + kPiDividedBy12 * acos(tmp);

  // Formula [2.12] in book p.31
  // 12 - t_sr = t_ss - 12
  // t_sr = 24 - t_ss
  double t_sr = 24 - t_ss;

  // Formula [2.13] in book p.31
  // day_length = 2 * (t_ss - 12)
  double day_length = 2 * (t_ss - kHoursHalfDay);

  return std::make_tuple(t_sr, t_ss, day_length);
}

std::tuple<double, double, double> MeteoInfo::CalculateDailySolarIrradiance(
    const double I_c_prime, const double a, const double b,
    const Climate::ZoneType climate_zone, const double day_length) {
  // Formula [2.14] in book p.33
  // I_t_d = I_et_d * (b_0 + b_1 * (s / DL))
  // b_0, b_1: empirical coefficients
  // s: duration of sunshine hours (hours)
  // DL: day length (hours)
  // s/DL: relative sunshine hours

  // Formula [2.23] in book p.37
  // I_et_d = 3600 * I_c_prime * (24 / π) * (a * acos(-a/b) + b * sqrt(1 -
  // (a/b)^2))
  // a: sin(λ) * sin(δ)
  // b: cos(λ) * cos(δ)
  double I_et_d = kSecsPerMin * kMinsPerHour * I_c_prime *
                  (kHoursPerDay / kPI) *
                  (a * acos(-a / b) + b * sqrt(1 - pow(a / b, 2)));

  // b_0 and b_1 are constants based on climate zones
  double b_0, b_1;
  // Not quite sure about the classification
  switch (climate_zone) {
    // Cold or temperate
    case Climate::TemperateOceanic:
    case Climate::TemperateContinental:
    case Climate::TemperateWithHumidWinters:
    case Climate::TemperateWithDryWinters:
    case Climate::Boreal:
    case Climate::Polar:
      b_0 = 0.18;
      b_1 = 0.55;
      break;
    // Dry tropical
    case Climate::TropicalWetAndDry:
    case Climate::DesertOrArid:
    case Climate::SteppeOrSemiArid:
    case Climate::SubtropicalDrySummer:
    case Climate::SubtropicalDryWinter:
      b_0 = 0.25;
      b_1 = 0.45;
      break;
    // Humid tropical
    case Climate::TropicalWet:
    case Climate::SubtropicalHumid:
      b_0 = 0.29;
      b_1 = 0.42;
      break;
    default:
      break;
  }

  // I_t_d = I_et_d * (b_0 + b_1 * (s / DL))
  // Assume "s/DL" = 1 here
  double s = day_length;
  double I_t_d = I_et_d * (b_0 + b_1 * (s / day_length));

  // Calculate diffuse irradiance
  // Formula [2.25] in book p.38
  double I_df_d;
  double sky_clearness = I_t_d / I_et_d;
  if (sky_clearness < 0.07) {
    // Formula: I_df_d/I_t_d = 1
    I_df_d = I_t_d;
  } else if (sky_clearness < 0.35) {
    // Formula: I_df_d/I_t_d = 1 - 2.3 * (I_t_d/I_et_d - 0.07)^2
    I_df_d = I_t_d * (1 - 2.3 * pow(sky_clearness - 0.07, 2));
  } else if (sky_clearness < 0.75) {
    // Formula: I_df_d/I_t_d = 1.33 - 1.46 * I_t_d / I_et_d
    I_df_d = I_t_d * (1.33 - 1.46 * sky_clearness);
  } else {
    // Formula: I_df_d/I_t_d = 0.23
    I_df_d = I_t_d * 0.23;
  }

  // Calculate direct irradiance
  // Formula [2.26] in book p.39
  double I_dr_d = I_t_d - I_df_d;

  return std::make_tuple(I_t_d, I_df_d, I_dr_d);
}

std::tuple<double, double, double> MeteoInfo::CalculateHourlySolarIrradiance(
    const double beta, const double I_c_prime, const double a, const double b,
    const double I_t_d, const int t_h) {
  // Formula [2.27] in book p.39
  // I_t = A * cos(2 * π * t_h / 24) + B
  // A = -b * ψ
  // B = a * ψ
  // a = sin(λ) * sin(δ)
  // b = cos(λ) * cos(δ)
  // ψ = (π * I_t_d / 86400) / (a * acos(-a / b) + b * sqrt(1 - (a / b)^2))
  double psi = (kPI * I_t_d / (kSecsPerMin * kMinsPerHour * kHoursPerDay)) /
               (a * acos(-a / b) + b * sqrt(1 - pow(a / b, 2)));
  double A = -b * psi;
  double B = a * psi;
  double I_t = A * cos(k2PI * t_h / kHoursPerDay) + B;

  // Calculate diffuse irradiance
  // Formula [2.22] in book p.37
  double I_et = I_c_prime * sin(beta);
  // Formula [2.31] in book p.40
  double sky_clearness = I_t / I_et;
  // Diffuse irradiance
  double I_df;
  // Constants presented in book p.40
  double R = 0.847f - 1.61 * sin(beta) + 1.04 * pow(sin(beta), 2);
  double K = (1.47 / R) / 1.66;
  if (sky_clearness <= 0.22) {
    // Formula: I_df/I_t = 1
    I_df = I_t;
  } else if (sky_clearness <= 0.35) {
    // Formula: I_df/I_t = 1 - 6.4 * (I_t/I_et - 0.22)^2
    I_df = I_t * (1 - 6.4 * pow(sky_clearness - 0.22, 2));
  } else if (sky_clearness <= K) {
    // Formula: I_df/I_t = 1.47 - 1.66 * (I_t / I_et)
    I_df = I_t * (1.47 - 1.66 * sky_clearness);
  } else {
    // Formula: I_df/I_t = R
    I_df = I_t * R;
  }

  // Calculate direct irradiance
  // Formula [2.32] in book p.41
  double I_dr = I_t - I_df;

  return std::make_tuple(I_t, I_df, I_dr);
}

double MeteoInfo::CalculateSaturatedVaporPressure(const double temperature) {
  // Formula [2.40] in book p.43
  // e_s(T_a) = 6.1078 * exp(17.269 * (T_a / (T_a + 237.3)))
  return 6.1078 * exp(17.269 * temperature / (temperature + 237.3));
}

double MeteoInfo::CalculateAirTemperature(double t_h, const double temp_min,
                                          const double temp_max,
                                          const double t_sr,
                                          const double t_ss) {
  // offset 1.5 hour after sunrise
  const double kOffset = 1.5;

  // add a day if t_h is before sunrise
  if (t_h < (t_sr + kOffset)) {
    t_h += 24.0;
  }

  if (t_h > t_ss) {
    // Formula [2.47] when t_h > t_ss in book p.50
    // T_a = T_set + ((T_min - T_set) * (t_h - t_ss) / ((t_sr + 1.5) + (24 -
    // t_ss)))

    // calling itself to get the temperature at sunset (T_set)
    double temp_sunset =
        temp_min + (temp_max - temp_min) *
                       sin(kPI * (t_ss - t_sr - kOffset) / (t_ss - t_sr));
    return temp_sunset + ((temp_min - temp_sunset) * (t_h - t_ss) /
                          ((t_sr + kOffset) + (kHoursPerDay - t_ss)));
  } else {
    // Formula [2.47] when t_h <= t_ss in book p.50
    // T_a = T_min + (T_max - T_min) * sin(π * (t_h - t_sr - 1.5) / (t_ss -
    // t_sr))
    return temp_min + (temp_max - temp_min) *
                          sin(kPI * (t_h - t_sr - kOffset) / (t_ss - t_sr));
  }
}

}  // namespace environment