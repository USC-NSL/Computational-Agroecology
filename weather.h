
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
 
    DayWeather( int TMax = 0, int TMin = 0,
            int RFall = 0) :
    temp_min_(TMax), temp_max_(TMin),
    rainfall_(RFall) { }
    private : 
    double temp_min_;
    double temp_max_;
    double rainfall_;
};

// Describe general full year weather information of the farm
class Weather {
 public:
    Weather(int TMax = 0, int TMin = 0, int MaxRF = 0, int MinRF = 0) :
    max_temp_year_(TMax), min_temp_year_(TMin),
    max_rainfall_year_(MaxRF), min_rainfall_year_(MinRF) { }

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