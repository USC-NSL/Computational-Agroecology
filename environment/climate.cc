#include "climate.h"

namespace environment {

// TODO: do some magical things to implement this.
Climate::Climate(const Config& config)
    : Climate(0, 0, 0, 0, TropicalWetAndDry) {}

Climate::Climate(const double temp_max, const double temp_min,
                 const double rain_max, const double rain_min,
                 const ZoneType type)
    : yearly_temperature(temp_max, temp_min),
      yearly_rainfall(rain_max, rain_min),
      climate_zone(type) {}

Climate::Climate(const MaxMinTemperature& yearly_temperature,
                 const MaxMinRainfall& yearly_rainfall, const ZoneType type)
    : yearly_temperature(yearly_temperature),
      yearly_rainfall(yearly_rainfall),
      climate_zone(type) {}

}  // namespace environment