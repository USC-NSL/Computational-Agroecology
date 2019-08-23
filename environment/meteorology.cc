#include "meteorology.h"

#include <cmath>
#include <ctime>

namespace environment {

Meteorology::Meteorology(
    const std::chrono::system_clock::time_point &local_time,
    const Location &location, const Climate::ZoneType climate_zone,
    const Weather &weather)
    : geo_location_(location), climate_zone_(climate_zone) {
  UpdateGeoData();
  Update(local_time, weather);
}

void Meteorology::Update(
    const std::chrono::system_clock::time_point &local_time,
    const Weather &weather) {
  time_t tt = std::chrono::system_clock::to_time_t(local_time);
  struct tm *tm = localtime(&tt);
  Update(tm->tm_yday + 1, tm->tm_hour, tm->tm_min, tm->tm_sec, weather);
}

void Meteorology::Update(const int day_of_year, const int local_hour,
                         const int local_minute, const int local_second,
                         const Weather &weather) {
  UpdateDayOfYear(day_of_year);
  UpdateLocalTime(local_hour, local_minute, local_second);
  UpdateWeather(weather);
}

void Meteorology::UpdateGeoData() {
  // Get the middle between two latitude boundaries.
  double latitude =
      (geo_location_.latitude_bottom + geo_location_.latitude_top) / 2.0;

  constant_caches_.observer_latitude = DegreeToRadians(latitude);
}

void Meteorology::UpdateDayOfYear(const int day_of_year) {
  constant_caches_.solar_declination = CalculateSolarDeclination(day_of_year);

  // Update day length
  DayLengthAndTimesSunriseSunset times = CalculateDayLength(
      constant_caches_.solar_declination, constant_caches_.observer_latitude);
  solar_hour_sunrise_ = times.solar_hour_sunrise;
  solar_hour_sunset_ = times.solar_hour_sunset;
  day_length_ = times.day_length;

  // Formula [2.18] in book p.36
  // Solar constant
  constexpr double I_c = 1370.0f;

  // Formula [2.21] in book p.37
  // ε_0 is approximated by the following equation
  // ε_0 ≈ 1 + 0.033 * cos(2 * π * (t_d - 10) / 365)
  double epsilon_0 =
      1 + 0.033 * cos(k2PI * (day_of_year - kDaysLeftPerYear) / kDaysPerYear);

  // Formula [2.19] in book p.37
  // I_c_prime = ε_0 * I_c a = sin(λ) * sin(δ) b = cos(λ) * cos(δ)
  constant_caches_.I_c_prime = epsilon_0 * I_c;

  // to solve integral sin(β), we need the following a and b
  constant_caches_.a = sin(constant_caches_.observer_latitude) *
                       sin(constant_caches_.solar_declination);
  constant_caches_.b = cos(constant_caches_.observer_latitude) *
                       cos(constant_caches_.solar_declination);

  // Update daily irradiance
  daily_solar_irradiance_ = CalculateDailySolarIrradiance(
      constant_caches_.I_c_prime, constant_caches_.a, constant_caches_.b,
      climate_zone_, day_length_);

  day_of_year_ = day_of_year;
}

void Meteorology::UpdateLocalTime(const int hour, const int minute,
                                  const int second) {
  int total_sec = second + (kSecsPerMin * minute) + (kSecsPerHour * hour);
  double local_hour = double(total_sec) / kSecsPerHour;

  double longitude =
      (geo_location_.longitude_left + geo_location_.longitude_right) / 2.0;

  // Local solar time (hours)
  double local_solar_time =
      CalculateLocalSolarTime(day_of_year_, local_hour, longitude);

  UpdateLocalSolarHour(local_solar_time);
}

void Meteorology::UpdateLocalSolarHour(const double solar_hour) {
  // The hour angle τ (radians)
  double hour_angle = CalculateHourAngle(solar_hour);

  // Update Solar Elevation
  solar_elevation_ =
      CalculateSolarElevation(constant_caches_.solar_declination, hour_angle,
                              constant_caches_.observer_latitude);
  // Update Solar Azimuth
  solar_azimuth_ = CalculateSolarAzimuth(constant_caches_.solar_declination,
                                         constant_caches_.observer_latitude,
                                         solar_elevation_, solar_hour);

  // Update hourly irradiance
  hourly_solar_irradiance_ = CalculateHourlySolarIrradiance(
      solar_elevation_, constant_caches_.I_c_prime, constant_caches_.a,
      constant_caches_.b, daily_solar_irradiance_.total, solar_hour);

  local_solar_hour_ = solar_hour;
}

void Meteorology::UpdateWeather(const Weather &weather) {
  // Update air temperature
  air_temperature_ = CalculateAirTemperature(
      local_solar_hour_, weather.temperature.min, weather.temperature.max,
      solar_hour_sunrise_, solar_hour_sunset_);

  // TODO: We need relative humidity from weather data to fill in this variable.
  // Declare a variable here just to make this compilable.
  double relative_humidity;

  // Update vapor pressure
  vapor_pressure_ = CalculateVaporPressure(air_temperature_, relative_humidity);
}

double Meteorology::DegreeToRadians(const double degree) {
  return degree * kPI / kPIforDegree;
}

double Meteorology::RadiansToDegree(const double radians) {
  return radians / kPI * kPIforDegree;
}

double Meteorology::CalculateSolarDeclination(const int day_of_year) {
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
  return A * cos(k2PI * (day_of_year + kDaysLeftPerYear) / kDaysPerYear);
}

double Meteorology::CalculateLocalSolarTime(const int day_of_year,
                                            const double local_hour,
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
  double B = k2PI * (day_of_year - 81) / (kDaysPerYear - 1);
  double EoT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
  return local_hour +
         ((gamma_sm - DegreeToRadians(longitude)) / kPiDividedBy12) +
         (EoT / kMinsPerHour);
}

double Meteorology::CalculateHourAngle(const double solar_hour) {
  // Formula [2.3] in book p.27
  // τ = (π / 12) * (t_h - 12)
  return kPiDividedBy12 * (solar_hour - kHoursHalfDay);
}

double Meteorology::CalculateSolarElevation(const double solar_declination,
                                            const double hour_angle,
                                            const double observer_latitude) {
  // Formula [2.8] in book p.30
  // β: Solar angle from horizontal in radians
  // sin(β) = sin(δ) * sin(λ) + cos(δ) * cos(λ) * cos(τ)
  double sin_beta =
      sin(solar_declination) * sin(observer_latitude) +
      cos(solar_declination) * cos(observer_latitude) * cos(hour_angle);
  return asin(sin_beta);
}

double Meteorology::CalculateSolarAzimuth(const double solar_declination,
                                          const double observer_latitude,
                                          const double solar_altitude,
                                          const double solar_hour) {
  // Formula [2.9], [2.10] in book p.30, p.31
  // β: Solar's altitude with respect to the observer
  // ϕ: Solar Azimuth
  // cos(α) = (sin(λ) * sin(β) - sin(δ)) / (cos(λ) * cos(β))
  // α = ϕ - π
  //   = ±acos((sin(λ) * sin(β) - sin(δ)) / (cos(λ) * cos(β)))
  double cos_alpha =
      (sin(observer_latitude) * sin(solar_altitude) - sin(solar_declination)) /
      (cos(observer_latitude) * cos(solar_altitude));
  cos_alpha = std::min(cos_alpha, 1.0);  // ensure cos(α) <= 1.0
  double alpha = acos(cos_alpha);

  // if before solar noon, alpha < 0
  if (solar_hour < kHoursHalfDay) {
    alpha = -alpha;
  }

  return kPI + alpha;
}

Meteorology::DayLengthAndTimesSunriseSunset Meteorology::CalculateDayLength(
    const double solar_declination, const double observer_latitude) {
  // Formula [2.11] in book p.31
  // t_ss = 12 + (12 / π) * acos(-(sin(δ) * sin(λ) / (cos(δ) * cos(λ))))

  // tmp = -((sin(δ) * sin(λ)) / (cos(δ) * cos(λ)))
  double tmp = -((sin(solar_declination) * sin(observer_latitude)) /
                 (cos(solar_declination) * cos(observer_latitude)));
  double t_ss = kHoursHalfDay + kPiDividedBy12 * acos(tmp);

  // Formula [2.12] in book p.31
  // 12 - t_sr = t_ss - 12
  // t_sr = 24 - t_ss
  double t_sr = 24 - t_ss;

  // Formula [2.13] in book p.31
  // day_length = 2 * (t_ss - 12)
  double day_length = 2 * (t_ss - kHoursHalfDay);

  return {t_sr, t_ss, day_length};
}

Meteorology::SolarIrradiance Meteorology::CalculateDailySolarIrradiance(
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
  double I_et_d = kSecsPerHour * I_c_prime * (kHoursPerDay / kPI) *
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
  // TODO: We assume "s/DL = 1" here, so `s` is assigned to `day_length`. We
  // need to figure out the true value of this.
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

  return {I_t_d, I_dr_d, I_df_d};
}

Meteorology::SolarIrradiance Meteorology::CalculateHourlySolarIrradiance(
    const double solar_altitude, const double I_c_prime, const double a,
    const double b, const double daily_total_solar_irradiance,
    const double solar_hour) {
  // Formula [2.27] in book p.39
  // I_t = A * cos(2 * π * t_h / 24) + B
  // A = -b * ψ
  // B = a * ψ
  // a = sin(λ) * sin(δ)
  // b = cos(λ) * cos(δ)
  // ψ = (π * I_t_d / 86400) / (a * acos(-a / b) + b * sqrt(1 - (a / b)^2))
  double psi = (kPI * daily_total_solar_irradiance / kSecsPerDay) /
               (a * acos(-a / b) + b * sqrt(1 - pow(a / b, 2)));
  double A = -b * psi;
  double B = a * psi;
  double I_t = A * cos(k2PI * solar_hour / kHoursPerDay) + B;

  // Calculate diffuse irradiance
  // Formula [2.22] in book p.37
  double I_et = I_c_prime * sin(solar_altitude);
  // Formula [2.31] in book p.40
  double sky_clearness = I_t / I_et;
  // Diffuse irradiance
  double I_df;
  // Constants presented in book p.40
  double R =
      0.847f - 1.61 * sin(solar_altitude) + 1.04 * pow(sin(solar_altitude), 2);
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

  return {I_t, I_dr, I_df};
}

Meteorology::VaporPressure Meteorology::CalculateVaporPressure(
    const double temperature, const double relative_humidity) {
  // Formula [2.40] in book p.43
  // e_s(T_a) = 6.1078 * exp(17.269 * (T_a / (T_a + 237.3)))
  double saturated = 6.1078 * exp(17.269 * temperature / (temperature + 237.3));

  // Formula [2.39] in book p.42
  // RH = 100 * (e_a / e_s(T_a))
  // e_a = RH * e_s(T_a) / 100
  double actual = relative_humidity * saturated / 100.0f;

  return {saturated, actual};
}

double Meteorology::CalculateAirTemperature(double solar_hour,
                                            const double temp_min,
                                            const double temp_max,
                                            const double solar_hour_sunrise,
                                            const double solar_hour_sunset) {
  // offset 1.5 hour after sunrise
  const double kOffset = 1.5;

  // add a day if t_h is before sunrise
  if (solar_hour < (solar_hour_sunrise + kOffset)) {
    solar_hour += 24.0;
  }

  if (solar_hour > solar_hour_sunset) {
    // get the temperature at sunset (T_set)
    // Formula [2.47] when t_h == t_ss in book p.50
    // T_set = T_min + (T_max - T_min) * sin(π * (t_ss - t_sr - 1.5) / (t_ss -
    // t_sr))
    double temp_sunset =
        temp_min +
        (temp_max - temp_min) *
            sin(kPI * (solar_hour_sunset - solar_hour_sunrise - kOffset) /
                (solar_hour_sunset - solar_hour_sunrise));

    // Formula [2.47] when t_h > t_ss in book p.50
    // T_a = T_set + ((T_min - T_set) * (t_h - t_ss) / ((t_sr + 1.5) + (24 -
    // t_ss)))
    return temp_sunset +
           ((temp_min - temp_sunset) * (solar_hour - solar_hour_sunset) /
            ((solar_hour_sunrise + kOffset) +
             (kHoursPerDay - solar_hour_sunset)));
  } else {
    // Formula [2.47] when t_h <= t_ss in book p.50
    // T_a = T_min + (T_max - T_min) * sin(π * (t_h - t_sr - 1.5) / (t_ss -
    // t_sr))
    return temp_min +
           (temp_max - temp_min) *
               sin(kPI * (solar_hour - solar_hour_sunrise - kOffset) /
                   (solar_hour_sunset - solar_hour_sunrise));
  }
}

}  // namespace environment