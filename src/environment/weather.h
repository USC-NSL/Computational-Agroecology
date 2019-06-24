#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_

#include <chrono>
#include <iostream>

#include "environment/climate.h"
#include "util/max_min_pair.h"

namespace environment {

// Moved from the original code base
// Weather specify condition on everyday
struct Weather {
  // We should be able to get the weather data from the climate and a specific
  // time point
  Weather(const environment::Climate& climate,
          const std::chrono::system_clock::time_point& time);

  Weather(const double temp_max, const double temp_min, const double rainfall);
  Weather(const MaxMinTemperature& temperature, const double rainfall);

  const MaxMinTemperature temperature;
  const double rainfall;
};

bool operator==(const Weather& lhs, const Weather& rhs);
std::ostream& operator<<(std::ostream& os, const Weather& weather);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_WEATHER_H_