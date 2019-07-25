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
 public:
  // keep everything public for simplicity
  std::vector<Face> faces;  // triangle faces
  GLuint vb_id;             // vertex buffer id
  int numTriangles;
  size_t material_id;
  GLuint texture_id;

  Mesh() {}
  ~Mesh() {}
  void LoadObjModel(const char *filename);
  void addFace(Face face) { faces.push_back(face); }

  // OpenGL rendering
  void render(std::vector<tinyobj::material_t> &materials, _462::Vector3 pos);
  void writeOpenGLBuffer(const std::vector<_462::Vector3> &vertices,
                         const std::vector<_462::Vector3> &normals,
                         const std::vector<_462::Vector2> &texcoords);
  void deleteOpenGLBuffer();

  // functions for photon mapping
  int getPhotons();

 private:
};

}  // namespace photonsimulator

}  // namespace simulator

#endif /* __MESH_H__ */