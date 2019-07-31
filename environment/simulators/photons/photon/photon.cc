#include "photon.h"

#include "Optimized-Photon-Mapping/src/math/math.hpp"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"

namespace simulator {

namespace photonsimulator {

bool CompareX(const Photon &i, const Photon &j) {
  return i.pos.x < j.pos.x;
}
bool CompareY(const Photon &i, const Photon &j) {
  return i.pos.y < j.pos.y;
}
bool CompareZ(const Photon &i, const Photon &j) {
  return i.pos.z < j.pos.z;
}

}  // namespace photonsimulator

}  // namespace simulator
