/*
    the class is a wrapper for mesh
    in order to load file and render in GL
*/

#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <cstring>
#include "../stdafx.h"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"
#include "mesh.h"

namespace simulator {

namespace photonsimulator {

class Texture {
 public:
  GLuint texture_id;
  unsigned char *buffer;
  int w, h;
  int comp;  // 3 = rgb, 4 = rgba

  Texture() = delete;
  Texture(GLuint texture_id_, int w_, int h_, int comp_)
      : texture_id(texture_id_), w(w_), h(h_), comp(comp_) {
    buffer = new unsigned char[w * h * 3];
  }
  ~Texture() { delete buffer; }
  Texture(const Texture &rhs) = delete;
  Texture(Texture &&rhs) noexcept : buffer(rhs.buffer) {
    buffer = rhs.buffer;
    rhs.buffer = nullptr;
  };
};

class Model {
 private:
  std::vector<_462::Vector3> vertices;
  std::vector<_462::Vector3> normals;
  std::vector<_462::Vector2> texcoords;
  std::vector<Mesh> meshes;
  std::map<std::string, GLuint> textures;
  std::vector<tinyobj::material_t> materials;
  std::vector<Texture> texture_infos;

  _462::Vector3 rel_pos;

 public:
  Model() = delete;
  void LoadObjModel(const char *filename);
  Model(const char *filename, _462::Vector3 pos = _462::Vector3(0.0, 0.0, 0.0))
      : rel_pos(pos) {
    LoadObjModel(filename);
  }
  ~Model();
  void setRelativePos(_462::Vector3 pos) { rel_pos = pos; }
  int getPhotons();
  int getTotalFaces();

  // add to buffer for OpenGL rendering
  void writeBuffer();
  void deleteBuffer();
  void render();
  const Texture &getTextureInfo(const GLuint &texture_id);

  // photon related
  bool IsInTriangle(const Face &face, const _462::Vector3 &p);
  _462::Vector3 GetIntersect(const Face &face, const _462::Vector3 &line_point,
                             const _462::Vector3 &line_dir);
  _462::real_t FindFirstIntersect(Face **face, Mesh **mesh,
                                  const _462::Vector3 &pos,
                                  const _462::Vector3 &dir);
  const _462::Vector3 GetFaceTextureColor(const Face &face, const Mesh &mesh,
                                          const _462::Vector3 &p);

 private:
};

// auxiliary functions
static bool FileExists(const std::string &abs_filename);
static std::string GetBaseDir(const std::string &filepath);
static bool hasSmoothingGroup(const tinyobj::shape_t &shape);
static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);
static void computeSmoothingNormals(
    const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
    std::map<int, _462::Vector3> &smoothVertexNormals);

}  // namespace photonsimulator

}  // namespace simulator

#endif