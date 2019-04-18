
// Copyright 2019

#ifndef WEATHER_H_
#define WEATHER_H_

#include "location.h"
#include <vector>
#include <algorithm>


//Temporary enum for CSV implementaiton
enum{
    TMIN_INDEX = 7, 
    TMAX_INDEX = 6, 
    RAINFALL_INDEX = 5
};






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
    public : 
    DayWeather( double TMax = 0, double TMin = 0,
            double RFall = 0) :
    temp_min_(TMax), temp_max_(TMin),
    rainfall_(RFall) { }
    double temp_max() {return temp_max_;}
    double temp_min() {return temp_min_;}
    double rainfall() {return rainfall_;}

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
    Weather(std::vector<DayWeather>); 
    double max_temp_year() {return max_temp_year_;}
    double min_temp_year() {return min_temp_year_;}
    double max_rainfall_year() {return max_rainfall_year_;}
    double min_rainfall_year() {return min_rainfall_year_;}
    std::vector<DayWeather> day_weather_list() {return day_weather_list_;}

 private: 
    double max_temp_year_;
    double min_temp_year_;
    double max_rainfall_year_;
    double min_rainfall_year_;
    std::vector<DayWeather> day_weather_list_; //different temperatures for different days
    //Location location_; (Does this need to be here?)
    ClimateZoneType climate_zone_;


};

class WeatherParser{
 public: 
    Weather GetWeatherByLocation(Location location);
};

#endif  // WEATHER_H_