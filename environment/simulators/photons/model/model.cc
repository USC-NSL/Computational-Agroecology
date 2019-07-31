#include "model.h"

#include <cmath>
#include <limits>

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STB_IMAGE_IMPLEMENTATION
#include "tinyobjloader/examples/viewer/stb_image.h"
#endif

#include "Optimized-Photon-Mapping/src/math/math.hpp"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"

#include "environment/simulators/photons/photon_simulator_config.h"
#include "environment/simulators/photons/stdafx.h"
#include "mesh.h"

namespace simulator {

namespace photonsimulator {

const char PathSeparator =
#if defined _WIN32 || defined __CYGWIN__
    '\\';
#else
    '/';
#endif

Texture::Texture(GLuint texture_id, int w, int h, int comp)
    : texture_id(texture_id), w(w), h(h), comp(comp) {
  buffer = new unsigned char[w * h * 3];
}

Texture::~Texture() {
  delete buffer;
}

Texture::Texture(const Texture &rhs)
    : texture_id(rhs.texture_id), w(rhs.w), h(rhs.h), comp(rhs.comp) {
  buffer = new unsigned char[w * h * 3];
  if (rhs.buffer != nullptr) {
    std::memcpy(buffer, rhs.buffer, sizeof(unsigned char) * w * h * 3);
  }
}

Texture::Texture(Texture &&rhs) noexcept
    : texture_id(rhs.texture_id),
      w(rhs.w),
      h(rhs.h),
      comp(rhs.comp),
      buffer(rhs.buffer) {
  rhs.buffer = nullptr;
}

Texture &Texture::operator=(const Texture &rhs) {
  texture_id = rhs.texture_id;
  w = rhs.w;
  h = rhs.h;
  comp = rhs.comp;

  if (buffer != nullptr) {
    delete buffer;
  }
  buffer = new unsigned char[w * h * 3];
  if (rhs.buffer != nullptr) {
    std::memcpy(buffer, rhs.buffer, sizeof(unsigned char) * w * h * 3);
  }

  return *this;
}

Texture &Texture::operator=(Texture &&rhs) noexcept {
  texture_id = rhs.texture_id;
  w = rhs.w;
  h = rhs.h;
  comp = rhs.comp;

  buffer = rhs.buffer;
  rhs.buffer = nullptr;

  return *this;
}

Model::~Model() {
  DeleteBuffer();
  std::cout << "model destroyed." << std::endl;
};

size_t Model::GetPhotons() const {
  size_t cnt = 0;
  for (const auto &mesh : meshes_) {
    cnt += mesh.GetPhotons();
  }
  return cnt;
}

size_t Model::GetTotalFaces() const {
  size_t cnt = 0;
  for (const auto &mesh : meshes_) {
    cnt += mesh.faces_.size();
  }
  return cnt;
}

bool Model::IsInTriangle(const Face &face, const _462::Vector3 &p) const {
  _462::Vector3 v0 = vertices_[face.vertex3.vertex_index] -
                     vertices_[face.vertex1.vertex_index];
  _462::Vector3 v1 = vertices_[face.vertex2.vertex_index] -
                     vertices_[face.vertex1.vertex_index];
  _462::Vector3 v2 = p - vertices_[face.vertex1.vertex_index];
  _462::real_t dot00 = _462::dot(v0, v0);
  _462::real_t dot01 = _462::dot(v0, v1);
  _462::real_t dot02 = _462::dot(v0, v2);
  _462::real_t dot11 = _462::dot(v1, v1);
  _462::real_t dot12 = _462::dot(v1, v2);
  _462::real_t inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
  _462::real_t u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
  if (u < 0 || u > 1)
    return false;
  _462::real_t v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
  if (v < 0 || v > 1)
    return false;
  return u + v <= 1;
}

_462::real_t Model::FindFirstIntersect(Face **face, Mesh **mesh,
                                       const _462::Vector3 &pos,
                                       const _462::Vector3 &dir) {
  Face *min_face = nullptr;
  Mesh *min_mesh = nullptr;
  _462::real_t distance = std::numeric_limits<double>::max();
  for (auto &mesh : meshes_) {
    for (auto &face : mesh.faces_) {
      _462::Vector3 intersect = GetIntersect(face, pos, dir);
      if (IsInTriangle(face, intersect) &&
          _462::distance(pos, intersect) < distance) {
        min_face = &face;
        min_mesh = &mesh;
        distance = _462::distance(pos, intersect);
      }
    }
  }
  *face = min_face;
  *mesh = min_mesh;
  return distance;
}

const _462::Vector3 Model::GetFaceTextureColor(const Face &face,
                                               const Mesh &mesh,
                                               const _462::Vector3 &p) const {
  _462::Vector2 texcoord =
      GetTexcoord(face, p - rel_pos_, vertices_, texcoords_);
  const Texture &texture_info = GetTextureInfo(mesh.texture_id_);
  int x =
      ((int)(texture_info.w * texcoord.x) % texture_info.w + texture_info.w) %
      texture_info.w;
  int y =
      ((int)(texture_info.h * texcoord.y) % texture_info.h + texture_info.h) %
      texture_info.h;
  unsigned char RGB[kNumOfChannels];
  float rgb[kNumOfChannels];
  memcpy(RGB, texture_info.buffer + kNumOfChannels * (x + y * texture_info.w),
         sizeof(unsigned char) * kNumOfChannels);
  for (int i = 0; i < kNumOfChannels; i++) {
    rgb[i] = RGB[i] / std::numeric_limits<unsigned char>::max();
  }
  return _462::Vector3(rgb);
}

_462::Vector3 Model::GetIntersect(const Face &face,
                                  const _462::Vector3 &line_point,
                                  const _462::Vector3 &line_dir) const {
  const _462::Vector3 &plane_normal = face.normal;
  _462::real_t d =
      _462::dot(vertices_[face.vertex1.vertex_index] + rel_pos_ - line_point,
                plane_normal) /
      _462::dot(line_dir, plane_normal);
  _462::Vector3 dir_norm = normalize(line_dir);
  return d * dir_norm + line_point;
}

// Ralph: This function looks way too long. Refactor it.
// wym: this function is not written by us, and I don't know the source
// (@Hangjie),the function is highly modified compared to the origin version.
void Model::LoadObjModel(const char *filename) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;

  // I/O operation
  {
    std::string base_dir = GetBaseDir(filename);
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials_, &warn, &err,
                                filename, base_dir.c_str());
    if (!warn.empty()) {
      std::cout << "WARN: " << warn << std::endl;
    }
    // Ralph: If error occurs, should it continue to run?
    // wym: I think so. cerr does not stop the program.
    if (!err.empty()) {
      std::cerr << err << std::endl;
    }
    // Append `default` material
    materials_.push_back(tinyobj::material_t());

    // Load diffuse textures
    {
      for (size_t m = 0; m < materials_.size(); m++) {
        const tinyobj::material_t &mp = materials_[m];

        if (mp.diffuse_texname.length() > 0) {
          // Only load the texture if it is not already loaded
          if (textures_.find(mp.diffuse_texname) == textures_.end()) {
            GLuint texture_id_;
            int w, h;
            int comp;

            std::string texture_filename = mp.diffuse_texname;
            if (!FileExists(texture_filename)) {
              // Append base dir.
              texture_filename = base_dir + mp.diffuse_texname;
              if (!FileExists(texture_filename)) {
                std::cerr << "Unable to find file: " << mp.diffuse_texname
                          << std::endl;
                exit(1);
              }
            }

            unsigned char *image = stbi_load(texture_filename.c_str(), &w, &h,
                                             &comp, STBI_default);
            if (!image) {
              std::cerr << "Unable to load texture: " << texture_filename
                        << std::endl;
              exit(1);
            }
            std::cout << "Loaded texture: " << texture_filename << ", w = " << w
                      << ", h = " << h << ", comp = " << comp << std::endl;

            glGenTextures(1, &texture_id_);
            glBindTexture(GL_TEXTURE_2D, texture_id_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            if (comp == 3) {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                           GL_UNSIGNED_BYTE, image);
            } else if (comp == 4) {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                           GL_UNSIGNED_BYTE, image);
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(image);
            textures_.insert(std::make_pair(mp.diffuse_texname, texture_id_));

            // test
            // only get RGB
            Texture texture(texture_id_, w, h, comp);
            glBindTexture(GL_TEXTURE_2D, texture_id_);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE,
                          texture.buffer);
            glBindTexture(GL_TEXTURE_2D, 0);
            texture_infos_.push_back(std::move(texture));
          }
        }
      }
    }
  }

  // Convert tiny_obj_loader format
  for (auto it = attrib.vertices.begin(); it != attrib.vertices.end(); it += 3)
    vertices_.push_back(_462::Vector3((_462::real_t)*it,
                                      (_462::real_t)*std::next(it),
                                      (_462::real_t)*std::next(it, 2)));
  for (auto it = attrib.normals.begin(); it != attrib.normals.end(); it += 3)
    normals_.push_back(_462::Vector3((_462::real_t)*it,
                                     (_462::real_t)*std::next(it),
                                     (_462::real_t)*std::next(it, 2)));
  // flip y texture coordinate
  for (auto it = attrib.texcoords.begin(); it != attrib.texcoords.end();
       it += 2)
    texcoords_.push_back(
        _462::Vector2((_462::real_t)*it, (_462::real_t)*std::next(it)));

  // Load mesh
  for (size_t s = 0; s < shapes.size(); s++) {
    Mesh mesh;

    // Check for smoothing group and compute smoothing normals
    std::map<int, _462::Vector3> smooth_vertex_normals;
    if (HasSmoothingGroup(shapes[s])) {
      std::cout << "Compute smoothingNormal for shape [" << s << "]"
                << std::endl;
      ComputeSmoothingNormals(attrib, shapes[s], smooth_vertex_normals);
    }

    for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
      Vertex v1(0, 0, 0), v2(0, 0, 0), v3(0, 0, 0);

      tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
      tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
      tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

      // update material_id for face
      int material_id_ = shapes[s].mesh.material_ids[f];
      if ((material_id_ < 0) ||
          (material_id_ >= static_cast<int>(materials_.size()))) {
        // Invaid material ID. Use default material.
        // Default material is added to the last item in `materials`.
        material_id_ = materials_.size() - 1;
      }

      float diffuse[3];
      for (size_t i = 0; i < 3; i++) {
        diffuse[i] = materials_[material_id_].diffuse[i];
      }

      // update texcoord index
      if (attrib.texcoords.size() > 0) {
        if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
            (idx2.texcoord_index < 0)) {
          // face does not contain valid uv index.
          // negative texture coordinate index points to 0
          v1.texcoord_index = -1;
          v2.texcoord_index = -1;
          v3.texcoord_index = -1;
        } else {
          // Don't forget to flip Y coord. for OpenGL rendering
          v1.texcoord_index = idx0.texcoord_index;
          v2.texcoord_index = idx1.texcoord_index;
          v3.texcoord_index = idx2.texcoord_index;
        }
      } else {
        // negative texture coordinate index points to 0
        v1.texcoord_index = -1;
        v2.texcoord_index = -1;
        v3.texcoord_index = -1;
      }

      // update vertex index
      v1.vertex_index = idx0.vertex_index;
      v2.vertex_index = idx1.vertex_index;
      v3.vertex_index = idx2.vertex_index;

      _462::Vector3 face_normal =
          CalcNormal(vertices_[v1.vertex_index], vertices_[v2.vertex_index],
                     vertices_[v3.vertex_index]);

      // update normal index
      {
        bool invalid_normal_index = false;
        if (attrib.normals.size() > 0) {
          int nf0 = idx0.normal_index;
          int nf1 = idx1.normal_index;
          int nf2 = idx2.normal_index;

          if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
            // normal index is missing from this face.
            invalid_normal_index = true;
          } else {
            v1.normal_index = nf0;
            v2.normal_index = nf1;
            v3.normal_index = nf2;
          }
        } else {
          invalid_normal_index = true;
        }

        if (invalid_normal_index && !smooth_vertex_normals.empty()) {
          // Use smoothing normals
          int f0 = idx0.vertex_index;
          int f1 = idx1.vertex_index;
          int f2 = idx2.vertex_index;

          if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
            v1.normal_index = normals_.size();
            normals_.push_back(smooth_vertex_normals[f0]);
            v2.normal_index = normals_.size();
            normals_.push_back(smooth_vertex_normals[f1]);
            v3.normal_index = normals_.size();
            normals_.push_back(smooth_vertex_normals[f2]);

            invalid_normal_index = false;
          }
        }

        if (invalid_normal_index) {
          v1.normal_index = v2.normal_index = v3.normal_index = normals_.size();
          normals_.push_back(face_normal);
        }
      }

      mesh.AddFace(Face(v1, v2, v3, face_normal, material_id_));
    }

    // update material_id_ for mesh
    // OpenGL viewer does not support texturing with per-face material.
    if (shapes[s].mesh.material_ids.size() > 0 &&
        shapes[s].mesh.material_ids.size() > s) {
      mesh.material_id_ =
          shapes[s].mesh.material_ids[0];  // use the material ID
                                           // of the first face.
    } else {
      mesh.material_id_ = materials_.size() - 1;  // = ID for default material.
    }

    // update texture_id_
    if ((mesh.material_id_ < materials_.size())) {
      std::string diffuse_texname =
          materials_[mesh.material_id_].diffuse_texname;
      if (textures_.find(diffuse_texname) != textures_.end()) {
        mesh.texture_id_ = textures_[diffuse_texname];
      }
    } else {
      mesh.texture_id_ = -1;
      std::cout << "Texture for " << filename << " not specified." << std::endl;
    }

    // update mesh
    meshes_.push_back(mesh);
  }
}

void Model::Render() {
  for (auto &mesh : meshes_) {
    mesh.Render(materials_, rel_pos_);
  }
}

void Model::WriteBuffer() {
  for (auto &mesh : meshes_) {
    mesh.WriteOpenGLBuffer(vertices_, normals_, texcoords_);
  }
}

void Model::DeleteBuffer() {
  for (auto &mesh : meshes_) {
    mesh.DeleteOpenGLBuffer();
  }
}

const Texture &Model::GetTextureInfo(const GLuint &texture_id_) const {
  for (const auto &texture_info : texture_infos_) {
    if (texture_info.texture_id == texture_id_) {
      return texture_info;
    }
  }
  assert(0);
}

// auxiliary functions

static bool FileExists(const std::string &abs_filename) {
  bool ret;
  FILE *fp = fopen(abs_filename.c_str(), "rb");
  if (fp) {
    ret = true;
    fclose(fp);
  } else {
    ret = false;
  }

  return ret;
}

static std::string GetBaseDir(const std::string &filepath) {
  if (filepath.find_last_of("/\\") != std::string::npos)
    return filepath.substr(0, filepath.find_last_of("/\\")) + PathSeparator;
  return "." + PathSeparator;
}

static bool HasSmoothingGroup(const tinyobj::shape_t &shape) {
  for (auto smoothing_group_id : shape.mesh.smoothing_group_ids) {
    if (smoothing_group_id > 0) {
      return true;
    }
  }
  return false;
}

static void ComputeSmoothingNormals(
    const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
    std::map<int, _462::Vector3> &smooth_vertex_normals) {
  smooth_vertex_normals.clear();

  for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
    // Get the three indexes of the face (all faces_ are triangular)
    tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
    tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
    tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

    // Get the three vertex indexes and coordinates
    int vertex_index[3];  // indexes
    float v[3][3];        // coordinates

    for (int k = 0; k < 3; k++) {
      vertex_index[0] = idx0.vertex_index;
      vertex_index[1] = idx1.vertex_index;
      vertex_index[2] = idx2.vertex_index;

      v[0][k] = attrib.vertices[3 * vertex_index[0] + k];
      v[1][k] = attrib.vertices[3 * vertex_index[1] + k];
      v[2][k] = attrib.vertices[3 * vertex_index[2] + k];
    }

    // Compute the normal of the face
    float normal[3];
    CalcNormal(normal, v[0], v[1], v[2]);

    // Add the normal to the three vertexes
    for (size_t i = 0; i < 3; ++i) {
      auto iter = smooth_vertex_normals.find(vertex_index[i]);
      if (iter != smooth_vertex_normals.end()) {
        // add
        iter->second.x += normal[0];
        iter->second.y += normal[1];
        iter->second.z += normal[2];
      } else {
        smooth_vertex_normals[vertex_index[i]].x = normal[0];
        smooth_vertex_normals[vertex_index[i]].y = normal[1];
        smooth_vertex_normals[vertex_index[i]].z = normal[2];
      }
    }

  }  // f

  // Normalize the normals, that is, make them unit vectors
  for (auto iter = smooth_vertex_normals.begin();
       iter != smooth_vertex_normals.end(); iter++) {
    normalize(iter->second);
  }

}  // ComputeSmoothingNormals

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
  float v10[3];
  v10[0] = v1[0] - v0[0];
  v10[1] = v1[1] - v0[1];
  v10[2] = v1[2] - v0[2];

  float v20[3];
  v20[0] = v2[0] - v0[0];
  v20[1] = v2[1] - v0[1];
  v20[2] = v2[2] - v0[2];

  N[0] = v20[1] * v10[2] - v20[2] * v10[1];
  N[1] = v20[2] * v10[0] - v20[0] * v10[2];
  N[2] = v20[0] * v10[1] - v20[1] * v10[0];

  float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
  if (len2 > 0.0f) {
    float len = sqrtf(len2);

    N[0] /= len;
    N[1] /= len;
    N[2] /= len;
  }
}

static _462::Vector3 CalcNormal(const _462::Vector3 &v0,
                                const _462::Vector3 &v1,
                                const _462::Vector3 &v2) {
  _462::Vector3 v10 = v1 - v0;
  _462::Vector3 v20 = v2 - v0;

  _462::Vector3 N(v20.y * v10.z - v20.z * v10.y, v20.z * v10.x - v20.x * v10.z,
                  v20.x * v10.y - v20.y * v10.z);
  return _462::normalize(N);
}

}  // namespace photonsimulator

}  // namespace simulator
