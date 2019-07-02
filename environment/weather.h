#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_

#include <chrono>
#include <iostream>

#include "climate.h"
#include "utility.h"

namespace environment {

// Moved from the original code base
// Weather specify condition on everyday
struct Weather {
  // We should be able to get the weather data from the climate and a specific
  // time point
  Weather(const environment::Climate& climate,
          const std::chrono::system_clock::time_point& time);

  Weather(const double temp_min, const double temp_max, const double rainfall);
  Weather(const MinMaxTemperature& temperature, const double rainfall);

  const MinMaxTemperature temperature;
  const double rainfall;
};

bool operator==(const Weather& lhs, const Weather& rhs);
std::ostream& operator<<(std::ostream& os, const Weather& weather);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_