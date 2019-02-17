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
    max_temperature_of_year_(MaxT),
    min_temperature_of_year_(MinT),
    today_max_temperature_(CTMax),
    today_min_temperature_(CTMin),
    max_rainfall_of_year_(MaxRF),
    min_rainfall_of_year_(MinRF) { }

    Location location() { return location_; }  // the location of this weather information

    int max_temperature_of_year() { return max_temperature_of_year_; }  // max temperature of the year

    int min_temperature_of_year() { return min_temperature_of_year_; }  // min temperature of the year

    int today_max_temperature() { return today_max_temperature_; }  // today's max temperature

    int today_min_temperature() { return today_min_temperature_; }  // today's min temperature

    int max_rainfall_of_year() { return max_rainfall_of_year_; }  // this year's max rainfall

    int min_rainfall_of_year() { return min_rainfall_of_year_; }  // this year's min rainfall


 private:
    Location location_;  // the location of this weather information

    int max_temperature_of_year_;  // max temperature of the year

    int min_temperature_of_year_;  // min temperature of the year

    int today_max_temperature_;  // today's max temperature

    int today_min_temperature_;  // today's min temperature

    int max_rainfall_of_year_;  // this year's max rainfall

    int min_rainfall_of_year_;  // this year's min rainfall

    ClimateZoneType ClimateZone_;  // the climate zone

 protected:
  void getWeatherByLocation() {};
};

#endif  // WEATHER_H_
