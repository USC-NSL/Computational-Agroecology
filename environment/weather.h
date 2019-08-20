#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_

#include <chrono>
#include <iostream>

#include "environment/location.h"
#include "environment/utility.h"

namespace environment {

// Moved from the original code base
// Weather specify condition on everyday
struct Weather {
  // We should be able to get the weather data from the location and a specific
  // time point
  Weather(const environment::Location &location,
          const std::chrono::system_clock::time_point &time);

  // TODO: These should be removed in the future
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