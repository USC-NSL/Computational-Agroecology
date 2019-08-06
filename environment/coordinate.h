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
  point_t ToVector() const;
};

bool operator==(const Coordinate &lhs, const Coordinate &rhs);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_COORDINATE_H_
