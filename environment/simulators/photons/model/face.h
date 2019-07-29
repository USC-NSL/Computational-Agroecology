// Ralph: refer to the google coding style to see what this should be
#ifndef __FACE_H__
#define __FACE_H__

#include "Optimized-Photon-Mapping/src/math/vector.hpp"

#include <vector>

#ifndef __MATERIAL_H__
#include "material.h"
#endif

namespace simulator {

namespace photonsimulator {

struct Vertex {
  int vertex_index;
  int normal_index;
  int texcoord_index;

  Vertex() = delete;
  Vertex(int vertex_index_, int normal_index_, int texcoord_index_)
      : vertex_index(vertex_index_),
        normal_index(normal_index_),
        texcoord_index(texcoord_index_) {}
  ~Vertex() {}
};

struct Face {
  Vertex vertex1, vertex2, vertex3;
  _462::Vector3 normal;  // face normal

  /**
   * material_id_ is for obj file.
   * material is self-defined structure.
   */
  int material_id_;
  Material material;  // TODO: how to import material info

  // count of photons
  size_t photons;

  Face() = delete;
  // Ralph: const
  Face(const Vertex &v1, const Vertex &v2, const Vertex &v3,
       const _462::Vector3 &normal, const int &material_id_ = 0)
      : vertex1(v1),
        vertex2(v2),
        vertex3(v3),
        photons(0),
        material_id_(material_id_),
        material(),
        normal(normal) {}
};

// given point on face, return its texture coordinate
_462::Vector2 GetTexcoord(const Face &face, const _462::Vector3 &pos,
                          const std::vector<_462::Vector3> &vertices_,
                          const std::vector<_462::Vector2> &texcoords_);

}  // namespace photonsimulator

}  // namespace simulator

#endif /* __FACE_H__ */
