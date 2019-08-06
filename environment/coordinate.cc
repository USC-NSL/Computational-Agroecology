#include "coordinate.h"
#include <assert.h>
#include <vector>

namespace environment {

Coordinate::Coordinate(const double x, const double y, const double z)
    : x(x), y(y), z(z) {}
Coordinate::Coordinate(std::vector<double> position) {
  assert(position.size() >= 3);
  x = position[0];
  y = position[1];
  z = position[2];
}

point_t Coordinate::ToVector() const {
  point_t position;
  position.push_back(x);
  position.push_back(y);
  position.push_back(z);
  return position;
}

// We assume that there are no two plants with same x,y coordinates
bool operator==(const Coordinate &lhs, const Coordinate &rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

}  // namespace environment
