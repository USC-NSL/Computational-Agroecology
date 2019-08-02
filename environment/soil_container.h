#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_

#include <vector>

#include "environment/soil.h"

namespace environment {

class SoilContainer {
 public:
  SoilContainer() {}
  Soil* FindSoilByCoord(double x, double y);
  std::vector<Soil *> FindSoilByCoords(double x, double y, double x_length,
                                       double y_length);

 private:
  std::vector<std::vector<Soil> > soils_;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SOILCONTAINER_H_
