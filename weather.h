// Copyright 2019

#ifndef WEATHER_H_
#define WEATHER_H_

#include "location.h"

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

// Describe general weather information of the farm
class Weather {
 public:
    Weather(int MaxT = 0, int MinT = 0, int CTMax = 0, int CTMin = 0,
            int CRF = 0, int MaxRF = 0, int MinRF = 0) :
    MaxTemperatureOfYear_(MaxT), MinTemperatureOfYear_(MinT),
    CurrentTempretureMax_(CTMax), CurrentTempretureMin_(CTMin),
    CurrentRainfall_(CRF), MaxRainfall_(MaxRF), MinRainfall_(MinRF) { }
    Location location_;
    int MaxTemperatureOfYear_;
    int MinTemperatureOfYear_;
    int CurrentTempretureMax_;
    int CurrentTempretureMin_;
    int CurrentRainfall_;
    int MaxRainfall_;
    int MinRainfall_;
    ClimateZoneType ClimateZone_;

 protected:
  void getWeatherByLocation() {};
};

#endif  // WEATHER_H_
