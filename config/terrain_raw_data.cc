#include "terrain_raw_data.h"

namespace config {

TerrainRawData::TerrainRawData(const size_t size) 
  : size(size) {}

bool operator==(const TerrainRawData &lhs, const TerrainRawData &rhs) {
  return (lhs.size == rhs.size);
}

}  // namespace config