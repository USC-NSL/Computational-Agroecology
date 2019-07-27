// // Ralph: refer to the google coding style to see what this should be
#ifndef __MESH_H__
#define __MESH_H__
#include <cassert>
#include <iostream>
#include "../stdafx.h"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"
#include "face.h"
#include "tinyobjloader/tiny_obj_loader.h"

namespace simulator {

namespace photonsimulator {

class Mesh {
 private:
  friend class Model;

  // Ralph: private member variable names should have trailing underscores.
  std::vector<Face> faces;  // triangle faces
  GLuint vb_id;             // vertex buffer id
  // Ralph: num_triangles_
  int numTriangles;
  size_t material_id;
  GLuint texture_id;

 public:
  // Ralph: Remove ctor and dtor
  Mesh() {}
  ~Mesh() {}
  // Ralph: Should this be private?
  void LoadObjModel(const char *filename);
  // Ralph: const Face& and `AddFace`
  void addFace(Face face) { faces.push_back(face); }

  // OpenGL rendering
  // Ralph: naming
  void render(const std::vector<tinyobj::material_t> &materials,
              const _462::Vector3 &rel_pos);
  void writeOpenGLBuffer(const std::vector<_462::Vector3> &vertices,
                         const std::vector<_462::Vector3> &normals,
                         const std::vector<_462::Vector2> &texcoords);
  void deleteOpenGLBuffer();

  // functions for photon mapping
  // Ralph: naming
  int getPhotons();
// Ralph: remove this
 private:
};

}  // namespace photonsimulator

}  // namespace simulator

#endif /* __MESH_H__ */