#include "face.h"

#include <cassert>

#include "Optimized-Photon-Mapping/src/math/math.hpp"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"

namespace simulator {

namespace photonsimulator {

_462::Vector2 GetTexcoord(const Face &face, const _462::Vector3 &pos,
                          const std::vector<_462::Vector3> &vertices,
                          const std::vector<_462::Vector2> &texcoords) {
  _462::Vector3 vertex12, vertex13, vertex1p;
  _462::real_t k12, k13;
  _462::Vector2 texcoord;
  vertex12 =
      vertices[face.vertex2.vertex_index] - vertices[face.vertex1.vertex_index];
  vertex13 =
      vertices[face.vertex3.vertex_index] - vertices[face.vertex1.vertex_index];
  vertex1p = pos - vertices[face.vertex1.vertex_index];
  k12 = _462::length(_462::cross(vertex13, vertex1p)) /
        _462::length(_462::cross(vertex12, vertex13));
  k13 = _462::length(_462::cross(vertex12, vertex1p)) /
        _462::length(_462::cross(vertex12, vertex13));
  texcoord = (k12 + k13) * texcoords[face.vertex1.texcoord_index] +
             (1 - k12) * texcoords[face.vertex2.texcoord_index] +
             (1 - k13) * texcoords[face.vertex3.texcoord_index];
  // flip x y coord
  texcoord.y = 1 - texcoord.y;
  texcoord.x = 1 - texcoord.x;
  return texcoord;
}

}  // namespace photonsimulator

}  // namespace simulator
