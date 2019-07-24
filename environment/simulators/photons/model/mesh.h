#ifndef __MESH_H__
#define __MESH_H__
#include <cassert>
#include <iostream>
#include "../stdafx.h"

#ifndef __VECTORS_H__
#include "../vectors.h"
#endif

#ifndef __FACE_H__
#include "face.h"
#endif

#ifndef TINY_OBJ_LOADER_H_
#include "../loader/tiny_obj_loader.h"
#endif

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
  void render(std::vector<tinyobj::material_t> &materials, Vector3 pos);
  void writeOpenGLBuffer(const std::vector<Vector3> &vertices,
                         const std::vector<Vector3> &normals,
                         const std::vector<Vector2> &texcoords);
  void deleteOpenGLBuffer();

  // functions for photon mapping
  void addOnePhoton(int index) { faces[index].photons++; }
  int getPhotons();

 private:
};

}  // namespace photonsimulator

}  // namespace simulator

#endif /* __MESH_H__ */