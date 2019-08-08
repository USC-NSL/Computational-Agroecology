#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_COORDINATE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_COORDINATE_H_

#include <vector>

#include "KDTree/KDTree.hpp"

namespace environment {

struct Coordinate {
  Coordinate(const double x, const double y, const double z = 0);
  Coordinate(std::vector<double> position);

  double x;
  double y;
  double z;
  point_t To3DVector() const;
  point_t To2DVector() const;
};

bool operator==(const Coordinate &lhs, const Coordinate &rhs);
bool IsSameLocationIn2D(const Coordinate &lhs, const Coordinate &rhs);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_COORDINATE_H_
