#include "climate.h"

namespace environment {

// TODO: do some magical things to implement this.
Climate::Climate(const Config& config)
    : Climate(0, 0, 0, 0, TropicalWetAndDry) {}

Climate::Climate(const double temp_min, const double temp_max,
                 const double rain_min, const double rain_max,
                 const ZoneType type)
    : yearly_temperature(temp_min, temp_max),
      yearly_rainfall(rain_min, rain_max),
      climate_zone(type) {}

Climate::Climate(const MinMaxTemperature& yearly_temperature,
                 const MinMaxRainfall& yearly_rainfall, const ZoneType type)
    : yearly_temperature(yearly_temperature),
      yearly_rainfall(yearly_rainfall),
      climate_zone(type) {}

bool operator==(const Climate& lhs, const Climate& rhs) {
  return (lhs.climate_zone == rhs.climate_zone) &&
         (lhs.yearly_temperature == rhs.yearly_temperature) &&
         (lhs.yearly_rainfall == rhs.yearly_rainfall);
}

}  // namespace environment