#include "neighbor.h"

#include <algorithm>

namespace simulator {

namespace photonsimulator {

Neighbor::Neighbor() : Neighbor(-1.0, 0) {}

Neighbor::Neighbor(const _462::real_t &s, const unsigned int e)
    : sq_dis(s), i(e){};

bool Neighbor::operator<(const Neighbor &rhs) {
  return sq_dis < rhs.sq_dis;
}

void AddNeighbor(const _462::Vector3 &p_pos, const _462::Vector3 &p_dir,
                 const _462::Vector3 &point, const _462::Vector3 &norm,
                 std::vector<Neighbor> *heap, const unsigned int index,
                 _462::real_t *distance, const _462::real_t kMaxDistance,
                 const int kNumberOfPhotonsNearby) {
  if (_462::dot(norm, p_dir) < 0.0f) {
    return;
  }
  _462::real_t e_dis = squared_distance(point, p_pos);
  if (e_dis <= kMaxDistance &&
      (heap->size() < kNumberOfPhotonsNearby || e_dis < *distance)) {
    if (heap->size() == kNumberOfPhotonsNearby) {
      heap->pop_back();
    }
    heap->push_back(Neighbor(e_dis, index));
    *distance = heap->begin()->sq_dis;
  }
}

_462::real_t GetSplitValueByIndex(const std::vector<Photon> &p,
                                  const unsigned int i, const int axis) {
  if (axis == kXAXIS) {
    return p[i].pos.x;
  }
  if (axis == kYAXIS) {
    return p[i].pos.y;
  }
  if (axis == kZAXIS) {
    return p[i].pos.z;
  }
  return 0.0;
}

_462::real_t GetSplitValueByPhoton(const _462::Vector3 &p, const int axis) {
  if (axis == kXAXIS) {
    return p.x;
  }
  if (axis == kYAXIS) {
    return p.y;
  }
  if (axis == kZAXIS) {
    return p.z;
  }
  return 0.0;
}

}  // namespace photonsimulator

}  // namespace simulator
