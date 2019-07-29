#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MODEL_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MODEL_H_
#include <cstring>
#include "../stdafx.h"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"
#include "mesh.h"

namespace simulator {

namespace photonsimulator {

struct Texture {
  // Ralph: Should all these variables public?
  // wym: so convert it yo structure ?
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
  std::vector<_462::Vector3> vertices_;
  std::vector<_462::Vector3> normals_;
  std::vector<_462::Vector2> texcoords_;
  std::vector<Mesh> meshes_;
  std::map<std::string, GLuint> textures_;
  std::vector<tinyobj::material_t> materials_;
  std::vector<Texture> texture_infos_;
  _462::Vector3 rel_pos_;

  void LoadObjModel(const char *filename);

 public:
  Model() = delete;
  Model(const char *filename,
        const _462::Vector3 &pos = _462::Vector3(0.0, 0.0, 0.0))
      : rel_pos_(pos) {
    LoadObjModel(filename);
  }
  Model(const Model &) = default;
  // Added `noexcept` qualifier to prevent `std::vector<Model>` from calling
  // copy ctor while resizing.
  Model(Model &&rhs) noexcept = default;

  ~Model();

  void setRelativePos(const _462::Vector3 &pos) { rel_pos_ = pos; }
  size_t GetPhotons() const;
  size_t GetTotalFaces() const;

  // Ralph: Should these member functions be public?
  // add to buffer for OpenGL rendering
  // wym: I think so, these function are not called right now, but would be
  // called by the IsRendering function
  void WriteBuffer();
  void DeleteBuffer();
  void Render();

  const Texture &GetTextureInfo(const GLuint &texture_id_) const;

  // photon related
  bool IsInTriangle(const Face &face, const _462::Vector3 &p) const;
  _462::Vector3 GetIntersect(const Face &face, const _462::Vector3 &line_point,
                             const _462::Vector3 &line_dir) const;
  // Ralph: same as above
  // wym: cannot convert const, since face should be modified by function
  // PhotonsModify
  _462::real_t FindFirstIntersect(Face **face, Mesh **mesh,
                                  const _462::Vector3 &pos,
                                  const _462::Vector3 &dir);
  const _462::Vector3 GetFaceTextureColor(const Face &face, const Mesh &mesh,
                                          const _462::Vector3 &p);
};

// auxiliary functions
static bool FileExists(const std::string &abs_filename);
static std::string GetBaseDir(const std::string &filepath);
static bool HasSmoothingGroup(const tinyobj::shape_t &shape);
// Ralph: Should parameters be changed to `Vector3`s? or are there existed
// functions in library for this?
// wym: I add following function, but I would suggest that do not modify the
// code.
static _462::Vector3 CalcNormal(const _462::Vector3 &v0,
                                const _462::Vector3 &v1,
                                const _462::Vector3 &v2);
static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);
static void ComputeSmoothingNormals(
    const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
    std::map<int, _462::Vector3> &smooth_vertex_normals);

}  // namespace photonsimulator

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MODEL_H_