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
  int vi;   // vertex index
  int vni;  // normal index
  int vti;  // texture coordinate index

  Vertex() = delete;
  Vertex(int vi_, int vni_, int vti_) : vi(vi_), vni(vni_), vti(vti_) {}
  ~Vertex() {}
};

struct Face {
  Vertex vertex1, vertex2, vertex3;
  _462::Vector3 normal;  // face normal

  /**
   * material_id is for obj file.
   * material is self-defined structure.
   */
  int material_id;
  Material material;  // TODO: how to import material info

  // count of photons
  int photons;

  Face() = delete;
  Face(Vertex v1, Vertex v2, Vertex v3, _462::Vector3 normal,
       int material_id_ = 0)
      : vertex1(v1),
        vertex2(v2),
        vertex3(v3),
        photons(0),
        material_id(material_id_),
        material(),
        normal(normal) {}
  ~Face() {}
};

// given point on face, return its texture coordinate
_462::Vector2 getTexcoord(const Face &face, const _462::Vector3 &pos,
                          const std::vector<_462::Vector3> &vertices,
                          const std::vector<_462::Vector2> &texcoords);

}  // namespace photonsimulator

}  // namespace simulator

#endif /* __FACE_H__ */
