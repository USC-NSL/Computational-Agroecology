#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CLIMATE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CLIMATE_H_

#include "config.h"
#include "utility.h"

namespace environment {

// Moved from the origin code base
struct Climate {
  enum ZoneType {
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

  Climate(const environment::Config& config);

  Climate(const double temp_max, const double temp_min, const double rain_max,
          const double rain_min, const ZoneType type);
  Climate(const MaxMinTemperature& yearly_temperature,
          const MaxMinRainfall& yearly_rainfall, const ZoneType type);

  const ZoneType climate_zone;
  const MaxMinTemperature yearly_temperature;
  const MaxMinRainfall yearly_rainfall;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CLIMATE_H_