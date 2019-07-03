#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_PHOTON_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_PHOTON_H_

#include "../vectors.h"

namespace simulator {

namespace photonsimulator {

class Photon {
 public:
  Vector3 dir, pos, power;
  char flag;
  Photon(const Vector3& dir, const Vector3& pos, const Vector3& power,
         const char flag = 0);
};

bool compare_x(const Photon& i, const Photon& j) const;
bool compare_y(const Photon& i, const Photon& j) const;
bool compare_z(const Photon& i, const Photon& j) const;

}  // namespace photonsimulator

}  // namespace simulator

#endif  // !COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_PHOTON_H_
