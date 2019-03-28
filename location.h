#ifndef AGROECOLOGY_LOCATION_H_
#define AGROECOLOGY_LOCATION_H_

class Location {
 public:
  Location(int latitude, int longitude)
      : latitude_(latitude), longitude_(longitude) {}

  double longitude() const { return longitude_; }

  double latitude() const { return latitude_; }

 private:
  double longitude_;
  double latitude_;
};

#endif  // AGROECOLOGY_LOCATION_H_
