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

point_t Coordinate::To3DVector() const {
  return {x, y, z};
}

point_t Coordinate::To2DVector() const {
  return {x, y};
}

bool operator==(const Coordinate &lhs, const Coordinate &rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

bool IsSameLocationIn2D(const Coordinate &lhs, const Coordinate &rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

}  // namespace environment

