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

  std::vector<Face> faces_;  // triangle faces_
  GLuint vb_id_;             // vertex buffer id
  int num_triangles_;
  size_t material_id_;
  GLuint texture_id_;

 public:
  // Ralph: Should this be private?
  void LoadObjModel(const char *filename);
  void AddFace(const Face &face) { faces_.push_back(face); }

  // OpenGL rendering
  void Render(const std::vector<tinyobj::material_t> &materials,
              const _462::Vector3 &rel_pos);
  void WriteOpenGLBuffer(const std::vector<_462::Vector3> &vertices_,
                         const std::vector<_462::Vector3> &normals_,
                         const std::vector<_462::Vector2> &texcoords_);
  void DeleteOpenGLBuffer();

  // functions for photon mapping
  size_t GetPhotons() const;
};

}  // namespace photonsimulator

}  // namespace simulator

#endif /* __MESH_H__ */