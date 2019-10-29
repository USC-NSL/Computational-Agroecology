#ifndef COMPUTATIONAL_AGROECOLOGY_CONFIG_TERRAIN_RAW_DATA_H_
#define COMPUTATIONAL_AGROECOLOGY_CONFIG_TERRAIN_RAW_DATA_H_

#include <iostream>

namespace config {

// Geographical location
struct TerrainRawData {
  TerrainRawData(const size_t size);

  const size_t size;
};

bool operator==(const TerrainRawData &lhs, const TerrainRawData &rhs);

}  // namespace config

#endif  // COMPUTATIONAL_AGROECOLOGY_CONFIG_LOCATION_H_