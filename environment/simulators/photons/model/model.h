/*
    the class is a wrapper for mesh
    in order to load file and Render in GL
*/
// Ralph: refer to the google coding style to see what this should be
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
  // Ralph: Should all these variables public?
  GLuint texture_id_;
  unsigned char *buffer;
  int w, h;
  int comp;  // 3 = rgb, 4 = rgba

  Texture() = delete;
  Texture(GLuint texture_id_, int w, int h, int comp);

  // Ralph: I think it would be better to still use vector. So we need to
  // define copy ctor (it is required for vector to compile its `push_back` even
  // though we are not using it). We just won't use it in runtime if we only do
  // `emplace_back` to this vector. On the other hand, if you could know about
  // how long would this vector be in advance, you could call
  // `vector<T>::reserve(const size_t)` to explicitly designate the amount of
  // memory that a vector should allocate. In that case, `std::vector` won't
  // resize frequently.
  Texture(const Texture &rhs);
  Texture(Texture &&rhs) noexcept;

  ~Texture();
};

class Model {
 private:
  // Ralph: all private member variable names should have a trailing underscore
  std::vector<_462::Vector3> vertices_;
  std::vector<_462::Vector3> normals_;
  std::vector<_462::Vector2> texcoords_;
  std::vector<Mesh> meshes;
  std::map<std::string, GLuint> textures;
  std::vector<tinyobj::material_t> materials;
  std::vector<Texture> texture_infos;

  _462::Vector3 rel_pos;

 public:
  Model() = delete;
  // Ralph: Should it be private?
  void LoadObjModel(const char *filename);
  // Ralph: const _462::Vector3&
  Model(const char *filename, _462::Vector3 pos = _462::Vector3(0.0, 0.0, 0.0))
      : rel_pos(pos) {
    LoadObjModel(filename);
  }
  Model(const Model &) = default;
  // Added `noexcept` qualifier to prevent `std::vector<Model>` from calling
  // copy ctor while resizing.
  Model(Model &&rhs) noexcept = default;

  ~Model();

  // Ralph: const _462::Vector3&
  void setRelativePos(_462::Vector3 pos) { rel_pos = pos; }
  // Ralph: size_t GetPhotons() const;
  size_t GetPhotons() const;
  // Ralph: size_t GetTotalFaces() const;
  int getTotalFaces();

  // Ralph: Should these member functions be public?
  // add to buffer for OpenGL rendering
  // Ralph: WriteBuffer();
  void writeBuffer();
  // Ralph: DeleteBuffer();
  void deleteBuffer();
  // Ralph: Render();
  void Render();

  // Ralph: const Texture &GetTextureInfo const (const GLuint &texture_id_);
  const Texture &getTextureInfo(const GLuint &texture_id_);

  // photon related
  // Ralph: bool IsInTriangle const (const Face &face, const _462::Vector3 &p);
  bool IsInTriangle(const Face &face, const _462::Vector3 &p);
  // Ralph: same as above
  _462::Vector3 GetIntersect(const Face &face, const _462::Vector3 &line_point,
                             const _462::Vector3 &line_dir);
  // Ralph: same as above
  _462::real_t FindFirstIntersect(Face **face, Mesh **mesh,
                                  const _462::Vector3 &pos,
                                  const _462::Vector3 &dir);
  // Ralph: same as above
  const _462::Vector3 GetFaceTextureColor(const Face &face, const Mesh &mesh,
                                          const _462::Vector3 &p);

  // Ralph: remove the below line
 private:
};

// auxiliary functions
static bool FileExists(const std::string &abs_filename);
static std::string GetBaseDir(const std::string &filepath);
// Ralph: HasSmoothingGroup
static bool hasSmoothingGroup(const tinyobj::shape_t &shape);
// Ralph: Should parameters be changed to `Vector3`s? or are there existed
// functions in library for this?
static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);
// Ralph: ComputeSmoothingNormals
static void computeSmoothingNormals(
    const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
    std::map<int, _462::Vector3> &smoothVertexNormals);

}  // namespace photonsimulator

}  // namespace simulator

#endif