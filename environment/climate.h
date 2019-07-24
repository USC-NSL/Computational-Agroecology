#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CLIMATE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CLIMATE_H_

#include "environment/config.h"
#include "environment/utility.h"

namespace environment {

// Moved from the origin code base
struct Climate {
  enum ZoneType {
    TropicalWetAndDry,
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

  // Construct this based on the `config`
  Climate(const environment::Config& config);

  // TODO: include other existing libraries to replace the current types with
  // unit types
  Climate(const double temp_min, const double temp_max, const double rain_min,
          const double rain_max, const ZoneType type);
  Climate(const MinMaxTemperature& yearly_temperature,
          const MinMaxRainfall& yearly_rainfall, const ZoneType type);

  const ZoneType climate_zone;
  const MinMaxTemperature yearly_temperature;
  const MinMaxRainfall yearly_rainfall;
};

bool operator==(const Climate& lhs, const Climate& rhs);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CLIMATE_H_