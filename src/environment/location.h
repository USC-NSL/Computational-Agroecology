#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_LOCATION_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_LOCATION_H_

namespace environment {

// Geographical location
struct Location {
  Location(const double longitude_left, const double longitude_right,
           const double latitude_top, const double latitude_bottom);

  const double longitude_left;
  const double longitude_right;
  const double latitude_top;
  const double latitude_bottom;
};

bool operator==(const Location& lhs, const Location& rhs);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_LOCATION_H_