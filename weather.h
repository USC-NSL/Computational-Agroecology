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
class DayWeather{ 
    int CurrentTemperatureMax_;
    int CurrentTemperatureMin_;
    int CurrentRainfall_;
    DayWeather( int CTMax = 0, int CTMin = 0,
            int CRF = 0) :
    CurrentTemperatureMax_(CTMax), CurrentTemperatureMin_(CTMin),
    CurrentRainfall_(CRF) { }
};

// Describe general weather information of the farm
class Weather {
 public:
    Weather(int MaxT = 0, int MinT = 0, int MaxRF = 0, int MinRF = 0) :
    MaxTemperatureOfYear_(MaxT), MinTemperatureOfYear_(MinT),
    MaxRainfallOfYear_(MaxRF), MinRainfallOfYear_(MinRF) { }

 private: 
    int MaxTemperatureOfYear_;
    int MinTemperatureOfYear_;
    int MaxRainfallOfYear_;
    int MinRainfallOfYear_;
    std::vector<DayWeather> DayWeatherList; //different temperatures for different days
    Location location_;
    ClimateZoneType ClimateZone_;


};

class WeatherParser{
 public: 
    Weather GetWeatherByLocation(Location location);
};

#endif  // WEATHER_H_
