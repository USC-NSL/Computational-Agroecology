#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_PHOTON_SIMULATOR_PHOTON_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_PHOTON_SIMULATOR_PHOTON_H_

#include "Optimized-Photon-Mapping/src/math/math.hpp"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"

namespace simulator {

namespace photonsimulator {

struct Photon {
  Photon(const _462::Vector3 &dir, const _462::Vector3 &pos,
         const _462::Vector3 &power, const char flag = 0)
      : dir(dir), pos(pos), power(power), flag(flag){};

  _462::Vector3 dir, pos, power;
  char flag;
};

bool CompareX(const Photon &i, const Photon &j);
bool CompareY(const Photon &i, const Photon &j);
bool CompareZ(const Photon &i, const Photon &j);

}  // namespace photonsimulator

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_PHOTON_SIMULATOR_PHOTON_H_
