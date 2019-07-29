#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MESH_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MESH_H_

#include <cassert>
#include <iostream>

#include "Optimized-Photon-Mapping/src/math/vector.hpp"
#include "tinyobjloader/tiny_obj_loader.h"

#include "../stdafx.h"
#include "face.h"

namespace simulator {

namespace photonsimulator {

class Mesh {
 public:
  void AddFace(const Face &face) { faces_.push_back(face); }

  // OpenGL rendering
  void Render(const std::vector<tinyobj::material_t> &materials_,
              const _462::Vector3 &rel_pos_);
  void WriteOpenGLBuffer(const std::vector<_462::Vector3> &vertices_,
                         const std::vector<_462::Vector3> &normals_,
                         const std::vector<_462::Vector2> &texcoords_);
  void DeleteOpenGLBuffer();

  // functions for photon mapping
  size_t GetPhotons() const;

 private:
  friend class Model;

  std::vector<Face> faces_;  // triangle faces_
  GLuint vb_id_;             // vertex buffer id
  int num_triangles_;
  size_t material_id_;
  GLuint texture_id_;

  void LoadObjModel(const char *filename);
};

}  // namespace photonsimulator

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MESH_H_