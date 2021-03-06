#ifndef COMPUTATIONAL_AGROECOLOGY_CONFIG_TERRAIN_RAW_DATA_H_
#define COMPUTATIONAL_AGROECOLOGY_CONFIG_TERRAIN_RAW_DATA_H_

#include <iostream>

namespace config {

// Raw data used to construct the Terrain object
struct TerrainRawData {
  TerrainRawData(const size_t size, const int yield);

  const size_t size;
  const int yield;
};

bool operator==(const TerrainRawData &lhs, const TerrainRawData &rhs);

}  // namespace config

#endif  // COMPUTATIONAL_AGROECOLOGY_CONFIG_TERRAIN_RAW_DATA_H_