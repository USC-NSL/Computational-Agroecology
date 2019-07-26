#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_

#include <vector>

#include "../photon_simulator_config.h"
#include "Optimized-Photon-Mapping/src/math/math.hpp"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"
#include "photon.h"

namespace simulator {

namespace photonsimulator {

class Neighbor {
 public:
  Neighbor();
  Neighbor(_462::real_t s, unsigned int e);
  _462::real_t sq_dis;
  unsigned int i;
  bool operator<(const Neighbor &rhs);
};

void AddNeighbor(const _462::Vector3 &p_pos, const _462::Vector3 &p_dir,
                 const _462::Vector3 &point, const _462::Vector3 &norm,
                 std::vector<Neighbor> &heap, unsigned int index,
                 _462::real_t &distance, const _462::real_t kMaxDistance,
                 int &size, const int kNumberOfPhotonsNeayby);
_462::real_t GetSplitValueByIndex(const std::vector<Photon> &p,
                                  const unsigned int i, const int axis);

_462::real_t GetSplitValueByPhoton(const _462::Vector3 &p, const int axis);

}  // namespace photonsimulator

}  // namespace simulator

#endif  // !COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_
