#include "photon_control.h"

void Photon_Control::PhotonAdd(const Vector3& dir, const Vector3& pos, const Vector3& power) {
	photons.push_back(Photon(dir, pos, power));
	photons_num++;
}

void Photon_Control::ConstructKDTree(unsigned int begin, unsigned int end) {
	if (end - begin == 0) {
		return;
	}
	if (end - begin == 1) {
		// indicate the leaf node
		photons[begin].flag = LEAF;
		return;
	}

	// calculate the variance
	unsigned median = begin + (end - begin) / 2;
	real_t x_avg = 0.0, y_avg = 0.0, z_avg = 0.0;
	real_t x_var = 0.0, y_var = 0.0, z_var = 0.0;
	real_t n = (real_t)(end - begin);
	std::vector<Photon>::iterator a = photons.begin() + begin;
	std::vector<Photon>::iterator b = photons.begin() + end;
	std::vector<Photon>::iterator it;
	for (it = a; it != b; ++it) {
		x_avg += (*it).pos.x;
		y_avg += (*it).pos.y;
		z_avg += (*it).pos.z;
	}
	for (it = a; it != b; ++it) {
		x_var += ((*it).pos.x - x_avg) * ((*it).pos.x - x_avg);
		y_var += ((*it).pos.y - y_avg) * ((*it).pos.y - y_avg);
		z_var += ((*it).pos.z - z_avg) * ((*it).pos.z - z_avg);
	}
	x_var /= n;
	y_var /= n;
	z_var /= n;

	// find the dimension with maximum variance 
	real_t max_var = std::max(std::max(x_var, y_var), z_var);

	// split the dimension and indicate the splitting axis
	if (max_var == x_var) {
		std::sort(photons.begin() + begin, photons.begin() + end, CompareX);
		photons[median].flag = X_AXIS;
	}
	if (max_var == y_var) {
		std::sort(photons.begin() + begin, photons.begin() + end, CompareY);
		photons[median].flag = Y_AXIS;
	}
	if (max_var == z_var) {
		std::sort(photons.begin() + begin, photons.begin() + end, CompareZ);
		photons[median].flag = Z_AXIS;
	}

	// recurse on left and right children 
	ConstructKDTree(begin, median);
	ConstructKDTree(median + 1, end);
}

void Photon_Control::ConstructKDTree() {
	ConstructKDTree(0, photons_num);
}

void Photon_Control::PhotonDel(int index) {
	photons.erase(photons.begin() + index);
	photons_num--;
}

void Photon_Control::PhotonEmit(Vector3 dir, Vector3 pos, Vector3 power) {
	PhotonAdd(dir, pos, power);
}

void Photon_Control::PhotonAbsorb(int index) {
	PhotonDel(index);
}

Neighbor* Photon_Control::LookuptKDTree(const Vector3& pos, const Vector3& normal, real_t max_d,  real_t& distance,  int& size, int num) {
	Neighbor* res = new Neighbor[num];
	EPSILON = max_d;
	NUM_PHOTON_RADIANCE = num;
	LookuptKDTree(pos, normal, res, 0, photons_num, distance, size);
	return res;
}

void Photon_Control::LookuptKDTree(const Vector3& p, const Vector3& norm, Neighbor* neighbors, unsigned int begin, unsigned int end, real_t& D, int& size) {
	if (begin == end)
		return;
	if (begin + 1 == end)
		// add photon at leaf node to neighbors heap
		return AddNeighbor(p, norm, neighbors, begin, D, size);

	unsigned int median = begin + (end - begin) / 2;
	// get splitting axis
	int flag = (photons[median]).flag;
	real_t split_value = GetSplit(median, flag);
	real_t p_value = GetP(p, flag);
	// check which side of the splitting axis to traverse
	if (p_value <= split_value) {
		// traverse left sub-tree first
		LookuptKDTree(p, norm, neighbors, begin, median, D, size);
		// add the current node
		AddNeighbor(p, norm, neighbors, median, D, size);
		// return if neighbors heap is full and all nodes in the
	// right sub-tree is further than those in neighbors heap
		if (size >= NUM_PHOTON_RADIANCE && (p_value - split_value) * (p_value - split_value) > D) {
			return;
		}
		// traverse right sub-tree
		return LookuptKDTree(p, norm, neighbors, median + 1, end, D, size);
	}
	else {
		// traverse right sub-tree first
		LookuptKDTree(p, norm, neighbors, median + 1, end, D, size);
		// add the current node
		AddNeighbor(p, norm, neighbors, median, D, size);
		// return if neighbors heap is full and all nodes in the
	// left sub-tree is further than those in neighbors heap
		if (size >= NUM_PHOTON_RADIANCE &&
			(p_value - split_value) * (p_value - split_value) > D) {
			return;
		}
		// traverse left sub-tree
		return LookuptKDTree(p, norm, neighbors, begin, median, D, size);
	}
}

void Photon_Control::HeapSwap(Neighbor* neighbors, int a, int b) {
	 unsigned a_i = (neighbors[a]).i;
	 real_t a_s = (neighbors[a]).sq_dis;

	 (neighbors[a]).i = (neighbors[b]).i;
	 (neighbors[a]).sq_dis = (neighbors[b]).sq_dis;
	 (neighbors[b]).i = a_i;
	 (neighbors[b]).sq_dis = a_s;
 }

void Photon_Control::HeapRemove(Neighbor* neighbors,  int& size) {
	 // move the last element to the root node so that
	 // the max element is replaced
	 (neighbors[0]).i = (neighbors[size - 1]).i;
	 (neighbors[0]).sq_dis = (neighbors[size - 1]).sq_dis;
	 size--;

	 int i = 0;
	 int left, right, bigger;
	 real_t i_val, left_val, right_val;
	 // swap the root node element downward until it has no children
	 // or both children have smaller values
	 while (1) {
		 left = 2 * i + 1;
		 right = 2 * i + 2;
		 if (left >= size && right >= size) {
			 // i is a leaf node (has no child)
			 return;
		 }
		 i_val = (neighbors[i]).sq_dis;
		 left_val = (left < size) ? (neighbors[left]).sq_dis : -1.0f;
		 right_val = (right < size) ? (neighbors[right]).sq_dis : -1.0f;
		 if (i_val >= left_val && i_val >= right_val) {
			 // i is bigger than both children
			 return;
		 }
		 if (left_val == -1.0 && right_val != -1.0) {
			 // i is smaller than right child 
			 HeapSwap(neighbors, i, right);
			 i = right;
		 }
		 if (left_val != -1.0 && right_val == -1.0) {
			 // i is smaller than left child 
			 HeapSwap(neighbors, i, left);
			 i = left;
		 }
		 else {
			 // i is smaller than at least one of the child
			 bigger = (left_val > right_val) ? left : right;
			 HeapSwap(neighbors, i, bigger);
			 i = bigger;
		 }
	 }

	 return;
 }

void Photon_Control::HeapAdd(Neighbor* neighbors, int& size, unsigned int e, real_t e_dis) {
	// insert a new element to the last element of max heap 
	int i = size;
	(neighbors[i]).i = e;
	(neighbors[i]).sq_dis = e_dis;
	size++;

	int parent;
	real_t i_val, parent_val;
	// swap the last element upward unitl it reaches the root node
	// or has a larger parent
	while (1) {
		if (i == 0) {
			// reached root node
			return;
		}
		parent = (i - 1) / 2;
		i_val = (neighbors[i]).sq_dis;
		parent_val = (neighbors[parent]).sq_dis;
		if (parent_val >= i_val) {
			// parent is bigger than i
			return;
		}

		HeapSwap(neighbors, i, parent);
		i = parent;
	}

	return;
}

void Photon_Control::AddNeighbor(const Vector3& p, const Vector3& norm, Neighbor* neighbors, unsigned int e, real_t& D, int& size) {
	if (dotresult(norm, photons[e].dir) < 0.0f) {
		return;
	}
	real_t e_dis = squared_distance(p, (photons[e]).pos);
	if (e_dis <= EPSILON && (size < NUM_PHOTON_RADIANCE || e_dis < D)) {
		// maintain the size of the max heap 
		if (size == NUM_PHOTON_RADIANCE) {
			HeapRemove(neighbors, size);
		}

		HeapAdd(neighbors, size, e, e_dis);

		// update the maximum square distance
		D = (neighbors[0]).sq_dis;
	}
}

real_t Photon_Control::GetSplit(unsigned int i, int axis) {
	if (axis == X_AXIS)
		return (photons[i]).pos.x;
	if (axis == Y_AXIS)
		return (photons[i]).pos.y;
	if (axis == Z_AXIS)
		return (photons[i]).pos.z;
	return 0.0;
}

real_t Photon_Control::GetP(Vector3 p, int axis) {
	if (axis == X_AXIS)
		return p.x;
	if (axis == Y_AXIS)
		return p.y;
	if (axis == Z_AXIS)
		return p.z;
	return 0.0;
}
