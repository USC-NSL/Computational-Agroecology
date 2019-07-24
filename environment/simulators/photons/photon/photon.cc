#include "photon.h"

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
Photon::Photon(const Vector3 &dir, const Vector3 &pos, const Vector3 &power,
               const char flag)
    : dir(dir), pos(pos), power(power), flag(flag) {}
}  // namespace photonsimulator
}  // namespace simulator
