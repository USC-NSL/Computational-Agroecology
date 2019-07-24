#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_

#include <vector>

#include "photon.h"
#include "../vectors.h"
#include "../photon_simulator_config.h"

namespace simulator {

namespace photonsimulator {

class Neighbor {
 public:
  Neighbor();
  Neighbor(real_t s, unsigned int e);
  real_t sq_dis;
  unsigned int i;
};

void HeapSwap(Neighbor* neighbors, int a, int b);
void HeapRemove(Neighbor* neighbors, int& size);
void HeapAdd(Neighbor* neighbors, int& size, unsigned int e, real_t e_dis);
void AddNeighbor(const Vector3& p_pos, const Vector3& p_dir,
                  const Vector3& point, const Vector3& norm,
                  Neighbor* neighbors, unsigned int index, real_t& distance,
                  const real_t kMaxDistance, int& size,
                  const int kNumberOfPhotonsNeayby);
real_t GetSplit(const std::vector<Photon>&p, const unsigned int i, const int axis);
// TODO: @Hangjie change to meaningful function name
real_t GetP(const Vector3& p, const int axis);

}  // namespace photonsimulator

}  // namespace simulator

#endif  // !COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_
