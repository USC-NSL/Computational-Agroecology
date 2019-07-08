#include "neighbor.h"

namespace simulator
{

namespace photonsimulator
{

Neighbor::Neighbor() {
  sq_dis = -1.0;
  i = 0;
};

Neighbor::Neighbor(real_t s, unsigned int e) {
  sq_dis = s;
  i = e;
};

void heap_swap(Neighbor* neighbors, int a, int b) {
  unsigned a_i = (neighbors[a]).i;
  real_t a_s = (neighbors[a]).sq_dis;
  (neighbors[a]).i = (neighbors[b]).i;
  (neighbors[a]).sq_dis = (neighbors[b]).sq_dis;
  (neighbors[b]).i = a_i;
  (neighbors[b]).sq_dis = a_s;
}

void heap_remove(Neighbor* neighbors, int& size) {
  (neighbors[0]).i = (neighbors[size - 1]).i;
  (neighbors[0]).sq_dis = (neighbors[size - 1]).sq_dis;
  size--;
  int i = 0;
  int left, right, bigger;
  real_t i_val, left_val, right_val;
  while (1) {
	left = 2 * i + 1;
	right = 2 * i + 2;
	if (left >= size && right >= size)
	  return;
	i_val = (neighbors[i]).sq_dis;
	left_val = (left < size) ? (neighbors[left]).sq_dis : -1.0f;
	right_val = (right < size) ? (neighbors[right]).sq_dis : -1.0f;
	if (i_val >= left_val && i_val >= right_val)
	  return;
	if (left_val == -1.0 && right_val != -1.0) {
	  heap_swap(neighbors, i, right);
	  i = right;
	}
	if (left_val != -1.0 && right_val == -1.0) {
	  heap_swap(neighbors, i, left);
	  i = left;
	}
	else {
	  bigger = (left_val > right_val) ? left : right;
	  heap_swap(neighbors, i, bigger);
	  i = bigger;
	}
  }
}

void heap_add(Neighbor* neighbors, int& size, unsigned int e, real_t e_dis) {
  int i = size;
  int parent;
  real_t i_val, parent_val;
  (neighbors[i]).i = e;
  (neighbors[i]).sq_dis = e_dis;
  size++;
  while (1) {
	if (i == 0)
	  return;
	parent = (i - 1) / 2;
	i_val = (neighbors[i]).sq_dis;
	parent_val = (neighbors[parent]).sq_dis;
	if (parent_val >= i_val)
	  return;
    heap_swap(neighbors, i, parent);
	i = parent;
  }
}

void add_neighbor(const Vector3& p_pos, const Vector3& p_dir, const Vector3& point, const Vector3& norm, Neighbor* neighbors, unsigned int e, real_t& D, const real_t EPSILON, int& size, const int NUM_PHOTON_RADIANCE) {
  if (dotresult(norm, p_dir) < 0.0f)
	return;
  real_t e_dis = squared_distance(point, p_pos);
  if (e_dis <= EPSILON && (size < NUM_PHOTON_RADIANCE || e_dis < D)) {
	if (size == NUM_PHOTON_RADIANCE)
	  heap_remove(neighbors, size);
    heap_add(neighbors, size, e, e_dis);
	D = (neighbors[0]).sq_dis;
  }
}

real_t get_split(std::vector<Photon>&p, unsigned int i, int axis) {
  if (axis == kXAXIS)
    return (p[i]).pos.x;
  if (axis == kYAXIS)
    return (p[i]).pos.y;
  if (axis == kZAXIS)
    return (p[i]).pos.z;
  return 0.0;
}

real_t get_p(Vector3 p, int axis) {
  if (axis == kXAXIS)
    return p.x;
  if (axis == kYAXIS)
    return p.y;
  if (axis == kZAXIS)
    return p.z;
  return 0.0;
}

} // namespace photonsimulator

} // namespace simulator
