#include "weather.h"

namespace environment {

// TODO: do some magical things to implement this
Weather::Weather(const environment::Climate& climate,
                 const std::chrono::system_clock::time_point& time)
    : Weather(0, 0, 0) {}

Weather::Weather(const double temp_max, const double temp_min,
                 const double rainfall)
    : temperature(temp_max, temp_min), rainfall(rainfall) {}

Weather::Weather(const MaxMinTemperature& temperature, const double rainfall)
    : temperature(temperature), rainfall(rainfall) {}

std::ostream& operator<<(std::ostream& os, const Weather& weather) {
  os << "Max temperature for today: " << weather.temperature.max << "\n";
  os << "Min temperature for today: " << weather.temperature.min << "\n";
  os << "rainfall for today: " << weather.rainfall << "\n";

  return os;
}

}  // namespace environment