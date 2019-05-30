#ifndef _PHOTON_H_
#define _PHOTON_H_
#ifndef __VECTORS_HPP__
#include "common/vectors.hpp"
#endif
class Photon {
public:
	Vector3 dir, pos, power;
	char flag;
	Photon(Vector3& dir, Vector3& pos, Vector3& power, char flag = 0);
};
#endif // !_PHOTON_H_
