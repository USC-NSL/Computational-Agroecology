#include "location.h"

namespace environment {

Location::Location(const double longitude_left, const double longitude_right,
                   const double latitude_top, const double latitude_bottom)
    : longitude_left(longitude_left),
      longitude_right(longitude_right),
      latitude_top(latitude_top),
      latitude_bottom(latitude_bottom) {}

bool operator==(const Location &lhs, const Location &rhs) {
  return (lhs.longitude_left == rhs.longitude_left) &&
         (lhs.longitude_right == rhs.longitude_right) &&
         (lhs.latitude_top == rhs.latitude_top) &&
         (lhs.latitude_bottom == rhs.latitude_bottom);
}

}  // namespace environment