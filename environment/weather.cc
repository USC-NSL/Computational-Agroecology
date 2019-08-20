#include "weather.h"

namespace environment {

// TODO: do some magical things to implement this
Weather::Weather(const environment::Location &location,
                 const std::chrono::system_clock::time_point &time)
    : Weather(0, 0, 0, 0, 0, 0) {}

Weather::Weather(const double total_sunshine_hour, const double air_temp_min,
                 const double air_temp_max, const double relative_humidity,
                 const double wind_speed, const double rainfall)
    : total_sunshine_hour(total_sunshine_hour),
      air_temperature(air_temp_min, air_temp_max),
      relative_humidity(relative_humidity),
      wind_speed(wind_speed),
      rainfall(rainfall) {}

Weather::Weather(const double total_sunshine_hour,
                 const MinMaxTemperature &air_temperature,
                 const double relative_humidity, const double wind_speed,
                 const double rainfall)
    : total_sunshine_hour(total_sunshine_hour),
      air_temperature(air_temperature),
      relative_humidity(relative_humidity),
      wind_speed(wind_speed),
      rainfall(rainfall) {}

bool operator==(const Weather &lhs, const Weather &rhs) {
  return (lhs.total_sunshine_hour == rhs.total_sunshine_hour) &&
         (lhs.air_temperature == rhs.air_temperature) &&
         (lhs.relative_humidity == rhs.relative_humidity) &&
         (lhs.wind_speed == rhs.wind_speed) && (lhs.rainfall == rhs.rainfall);
}

std::ostream &operator<<(std::ostream &os, const Weather &weather) {
  os << "Total sunshine hour: " << weather.total_sunshine_hour << " hour(s)\n";
  os << "Air temperature: "
     << "Min: " << weather.air_temperature.min << "°C "
     << "Max: " << weather.air_temperature.max << "°C\n";
  os << "Relative humidity: " << weather.relative_humidity << "%\n";
  os << "Wind speed: " << weather.wind_speed << "m s-1\n";
  os << "Amount of Rainfall" << weather.rainfall << "mm day-1\n";

  return os;
}

}  // namespace environment