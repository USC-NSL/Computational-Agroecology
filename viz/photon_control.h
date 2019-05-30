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
#define EPSILON 1.0
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
	std::vector<Photon> photons; 
	int photons_num;
	void heap_swap(Neighbor* neighbors, int a, int b);
	void heap_remove(Neighbor* neighbors, int* size);
	void heap_add(Neighbor* neighbors, int* size, unsigned int e, real_t e_dis); 
	void add_neighbor(Vector3 p, Vector3 norm, Neighbor* neighbors, unsigned int e, real_t* D, int* size);
	real_t get_split(unsigned int i, int axis);
	real_t get_p(Vector3 p, int axis);
	Vector3 intersect;
	Vector3 get_Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3); 
	Vector3 get_Intersact(const Vector3& plane_normal, const Vector3& plane_point, const Vector3& line_point, const Vector3& line_dir);
	bool in_Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 p);
	void photon_add(Vector3 dir, Vector3 pos, Vector3 power);
	void photon_del(int index);
	void construct_kdtree(unsigned int begin, unsigned int end);
	void lookup_kdtree(Vector3 p, Vector3 norm, Neighbor* neighbors, unsigned int begin, unsigned int end, real_t* D, int* size);

public:
	Photon_Control() :photons_num(0), intersect(Vector3(0.0f, 0.0f, 0.0f)) {}
	int size() { return photons_num; }
	Photon& operator[](int index) { return photons[index]; }
	void photon_emit(Vector3 dir, Vector3 pos, Vector3 power);
	Vector3& get_intersect() { return intersect; }
	bool photon_intersect(int index, Vector3 normal, Vector3 a, Vector3 b, Vector3 c);
	Vector3 photon_reflect(int index, Vector3 normal);
	Vector3 photon_refract(int index, Vector3 normal, real_t coef);
	Neighbor* lookup_kdtree(Vector3 pos, Vector3 normal, real_t* distance, int* size, int num);
	void photon_absorb(int index);
	void construct_kdtree();
};
#endif // !_PHOTON_CONTROL_H_