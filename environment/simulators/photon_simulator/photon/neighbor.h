#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_

#include "../photon_simulator_config.h"
#include "../vectors.h"

namespace simulator
{

namespace photonsimulator
{

class Neighbor {
 public:
  Neighbor();
  Neighbor(real_t s, unsigned int e);
  real_t sq_dis;
  unsigned int i;
};

void heap_swap(Neighbor* neighbors, int a, int b);
void heap_remove(Neighbor* neighbors, int& size);
void heap_add(Neighbor* neighbors, int& size, unsigned int e, real_t e_dis);
void add_neighbor(const Vector3& p_pos, const Vector3& p_dir, const Vector3& point, const Vector3& norm, Neighbor* neighbors, unsigned int e, real_t& D, const real_t EPSILON, int& size, const int NUM_PHOTON_RADIANCE);
real_t get_split(unsigned int i, int axis);
real_t get_p(Vector3 p, int axis);

} // namespace photonsimulator

} // namespace simulator

#endif // !COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_NEIGHBOR_H_
