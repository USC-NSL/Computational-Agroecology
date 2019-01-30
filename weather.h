#ifndef COMPUTATIONAL_AGROECOLOGY_WEATHER_H
#define COMPUTATIONAL_AGROECOLOGY_WEATHER_H

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
class weather {

 protected:
  Location location_;
  double MaxTemperatureOfYear_;
  double MinTemperatureOfYear_;
  double AnnualRainfall_;
  ClimateZoneType ClimateZone_;
};


#endif //COMPUTATIONAL_AGROECOLOGY_WEATHER_H
