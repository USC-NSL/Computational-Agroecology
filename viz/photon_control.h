#ifndef _PHOTON_CONTROL_H_
#define _PHOTON_CONTROL_H_
#ifndef _PHOTON_MAPPING_H_
#include "photon.h"
#endif
#ifndef __MESH_H__
#include "model/mesh.h"
#endif
#ifndef _VECTOR_
#include <vector>
#endif
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
// indicate the leaf node
#define LEAF 3
inline bool compare_x(Photon i, Photon j) {
	return i.pos.x < j.pos.x;
}
inline bool compare_y(Photon i, Photon j) {
	return i.pos.y < j.pos.y;
}
inline bool compare_z(Photon i, Photon j) {
	return i.pos.z < j.pos.z;
}
class Neighbor
{
public:
	Neighbor() {
		sq_dis = -1.0;
		i = 0;
	};
	Neighbor(real_t s, unsigned int e) {
		sq_dis = s;
		i = e;
	};
	// squared distance from photon to intersection
	real_t sq_dis;
	// index of photon in map
	unsigned int i;
};
class Photon_Control {
private:
	int NUM_PHOTON_RADIANCE;
	real_t EPSILON;
	std::vector<Photon> photons; 
	int photons_num;
	void heap_swap(Neighbor* neighbors, int a, int b);
	void heap_remove(Neighbor* neighbors,  int& size);
	void heap_add(Neighbor* neighbors, int& size, unsigned int e, real_t e_dis); 
	void add_neighbor(const Vector3& p, const Vector3& norm, Neighbor* neighbors, unsigned int e, real_t& D, int& size);
	real_t get_split(unsigned int i, int axis);
	real_t get_p(Vector3 p, int axis);
	void photon_add(const Vector3& dir, const Vector3& pos, const Vector3& power);
	void photon_del(int index);
	void construct_kdtree(unsigned int begin, unsigned int end);
	void lookup_kdtree(const Vector3& p, const Vector3& norm, Neighbor* neighbors, unsigned int begin, unsigned int end, real_t& D, int& size);

public:
	Photon_Control() :photons_num(0) {}
	int size() { return photons_num; }
	Photon& operator[](int index) { return photons[index]; }
	void photon_emit(Vector3 dir, Vector3 pos, Vector3 power);
	Neighbor* lookup_kdtree(const Vector3& pos, const Vector3& normal, real_t max_d, real_t& distance, int& size, int num);
	void photon_absorb(int index);
	void construct_kdtree();
};
#endif // !_PHOTON_CONTROL_H_