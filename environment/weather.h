#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_

#include <chrono>
#include <iostream>

#include "config/location.h"
#include "environment/utility.h"

namespace environment {

// Represents everyday weather
struct Weather {
  Weather(const double total_sunshine_hour, const double air_temp_min,
          const double air_temp_max, const double relative_humidity,
          const double wind_speed, const double rainfall);
  Weather(const double total_sunshine_hour,
          const MinMaxTemperature &air_temperature,
          const double relative_humidity, const double wind_speed,
          const double rainfall);

  const double total_sunshine_hour;
  const MinMaxTemperature air_temperature;
  const double relative_humidity;
  const double wind_speed;
  const double rainfall;
};

bool operator==(const Weather &lhs, const Weather &rhs);
std::ostream &operator<<(std::ostream &os, const Weather &weather);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_