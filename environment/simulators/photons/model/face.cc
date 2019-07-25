#include "face.h"
#include <cassert>
#include "Optimized-Photon-Mapping/src/math/math.hpp"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"

namespace simulator {

namespace photonsimulator {

_462::Vector2 Face::getTexcoord(_462::Vector3 pos,
                                const std::vector<_462::Vector3> &vertices,
                                const std::vector<_462::Vector2> &texcoords) {
  _462::Vector3 vertex12, vertex13, vertex1p;
  _462::real_t k12, k13;
  _462::Vector2 texcoord;
  vertex12 = vertices[vertex2.vi] - vertices[vertex1.vi];
  vertex13 = vertices[vertex3.vi] - vertices[vertex1.vi];
  vertex1p = pos - vertices[vertex1.vi];
  k12 = _462::length(_462::cross(vertex13, vertex1p)) /
        _462::length(_462::cross(vertex12, vertex13));
  k13 = _462::length(_462::cross(vertex12, vertex1p)) /
        _462::length(_462::cross(vertex12, vertex13));
  texcoord = (k12 + k13) * texcoords[vertex1.vti] +
             (1 - k12) * texcoords[vertex2.vti] +
             (1 - k13) * texcoords[vertex3.vti];
  // flip x y coord
  texcoord.y = 1 - texcoord.y;
  texcoord.x = 1 - texcoord.x;
  return texcoord;
}

}  // namespace photonsimulator

}  // namespace simulator
