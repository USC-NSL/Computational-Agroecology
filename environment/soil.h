#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_

namespace environment {

struct Soil {
  enum Texture { CLAY = 0, SILT, SAND };

  Soil(const Texture texture, const double pH, const double salinity,
       const double organic_matter, const double water_content);

  Texture texture;
  double pH;
  double salinity;
  double organic_matter;

  struct Nutrients {
    double nitrogen;
    double phosphorus;
    double potassium;
    double calcium;
    double magnesium;
    double sulfur;
  };

  double water_content;
};

bool operator==(const Soil& lhs, const Soil& rhs);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOIL_H_