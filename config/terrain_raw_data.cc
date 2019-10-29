#include "terrain_raw_data.h"

namespace config {

TerrainRawData::TerrainRawData(const size_t size, const int yield)
    : size(size), yield(yield) {}

bool operator==(const TerrainRawData &lhs, const TerrainRawData &rhs) {
  return (lhs.size == rhs.size) && (lhs.yield == rhs.yield);
}

}  // namespace config