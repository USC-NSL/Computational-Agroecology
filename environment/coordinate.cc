#include "coordinate.h"

#include <assert.h>
#include <vector>

namespace environment {

Coordinate::Coordinate(const double x, const double y, const double z)
    : x(x), y(y), z(z) {}

Coordinate::Coordinate(const std::vector<double> &position) {
  x = position.size() >= 1 ? position[0] : 0.0;
  y = position.size() >= 2 ? position[1] : 0.0;
  z = position.size() >= 3 ? position[2] : 0.0;
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
