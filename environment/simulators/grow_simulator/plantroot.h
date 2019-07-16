#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_PLANTROOT_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_PLANTROOT_H_

#include "../vectors.h"

namespace simulator {

namespace growsimulator {

struct PlantRoot {
 Vector3 pos;
 Vector3 dir;
 Vector3 norm;
 real_t length;
 PlantRoot(const Vector3& pos, const Vector3& dir, const Vector3& norm, const real_t length)
  :pos(pos), dir(dir), norm(norm), length(length) {}
};

}  // namespace growsimulator

}  // namespace simulator
#endif // !COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_PLANTROOT_H_