#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MODEL_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MODEL_H_
#include <cstring>

#include "Optimized-Photon-Mapping/src/math/vector.hpp"

#include "environment/simulators/photons/stdafx.h"
#include "mesh.h"

namespace simulator {

namespace photonsimulator {

struct Texture {
  Texture() = delete;
  Texture(GLuint texture_id, int w, int h, int comp);

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
  Texture &operator=(const Texture &rhs);
  Texture &operator=(Texture &&rhs) noexcept;

  ~Texture();

  GLuint texture_id;
  unsigned char *buffer;
  int w, h;
  int comp;  // 3 = rgb, 4 = rgba
};

class Model {
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

  void set_rel_pos(const _462::Vector3 &rel_pos) { rel_pos_ = rel_pos; }
  size_t GetPhotons() const;
  size_t GetTotalFaces() const;

  void WriteBuffer();
  void DeleteBuffer();
  void Render();

  const Texture &GetTextureInfo(const GLuint &texture_id) const;

  // photon related
  bool IsInTriangle(const Face &face, const _462::Vector3 &p) const;
  _462::Vector3 GetIntersect(const Face &face, const _462::Vector3 &line_point,
                             const _462::Vector3 &line_dir) const;
  _462::real_t FindFirstIntersect(Face **face, Mesh **mesh,
                                  const _462::Vector3 &pos,
                                  const _462::Vector3 &dir);
  const _462::Vector3 GetFaceTextureColor(const Face &face, const Mesh &mesh,
                                          const _462::Vector3 &p) const;

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