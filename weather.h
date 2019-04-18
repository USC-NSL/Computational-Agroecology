
// Copyright 2019

#ifndef WEATHER_H_
#define WEATHER_H_

#include "location.h"
#include <vector>

// Represent the climate zone
enum ClimateZoneType {
  TropicalWerAndDry,
  TropicalWet,
  DesertOrArid,
  SteppeOrSemiArid,
  SubtropicalHumid,
  SubtropicalDrySummer,
  SubtropicalDryWinter,
  TemperateOceanic,
  TemperateContinental,
  TemperateWithHumidWinters,
  TemperateWithDryWinters,
  Boreal,
  Polar
};
//Describes the day during a individual day
class DayWeather{
 public:
  explicit DayWeather( int TMax = 0, int TMin = 0,
                       int RFall = 0) :
      temp_min_(TMax), temp_max_(TMin),
      rainfall_(RFall) { }
  double getMinTemp() const {
    return temp_min_;
  }
  double getMaxTemp() const {
    return temp_max_;
  }
  double getRainfall() const {
    return rainfall_;
  }
 private :
  double temp_min_;
  double temp_max_;
  double rainfall_;
};

// Describe general full year weather information of the farm
class Weather {
 public:
  explicit Weather(int TMax = 0, int TMin = 0, int MaxRF = 0, int MinRF = 0) :
      max_temp_year_(TMax), min_temp_year_(TMin),
      max_rainfall_year_(MaxRF), min_rainfall_year_(MinRF) {}

  Weather(const Weather& other): max_temp_year_(other.max_temp_year_),
                                 min_temp_year_(other.min_temp_year_),
                                 max_rainfall_year_(other.max_rainfall_year_),
                                 min_rainfall_year_(other.min_rainfall_year_),
                                 climate_zone_(other.climate_zone_) {}

  int max_temp_year() {return max_temp_year_;}
  int min_temp_year() {return min_temp_year_;}
  int max_rainfall_year() {return max_rainfall_year_;}
  int min_rainfall_year() {return min_rainfall_year_;}

  std::vector<DayWeather> day_weather_list() {return day_weather_list_;}

 private:
  int max_temp_year_;
  int min_temp_year_;
  int max_rainfall_year_;
  int min_rainfall_year_;
  std::vector<DayWeather> day_weather_list_; //different temperatures for different days
  //Location location_; (Does this need to be here?)
  ClimateZoneType climate_zone_;


};

class WeatherParser{
 public:
  Weather GetWeatherByLocation(Location location);
};

#endif  // WEATHER_H_
