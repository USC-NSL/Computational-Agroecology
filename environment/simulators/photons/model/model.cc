#include "model.h"
#include "mesh.h"
#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STB_IMAGE_IMPLEMENTATION
#include "tinyobjloader/examples/viewer/stb_image.h"
#endif
#include <cmath>
#include "../stdafx.h"
#include "Optimized-Photon-Mapping/src/math/math.hpp"
#include "Optimized-Photon-Mapping/src/math/vector.hpp"

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
  // Ralph: This line should be removed. This just lets you experience a bit.
  std::cout << "Copy\n";
  buffer = new unsigned char[w * h * 3];
  std::memcpy(buffer, rhs.buffer, sizeof(unsigned char) * w * h * 3);
}

Texture::Texture(Texture &&rhs) noexcept
    : texture_id(rhs.texture_id),
      w(rhs.w),
      h(rhs.h),
      comp(rhs.comp),
      buffer(rhs.buffer) {
  rhs.buffer = nullptr;
}

Model::~Model() {
  deleteBuffer();
  std::cout << "model destroyed." << std::endl;
};

int Model::getPhotons() {
  int cnt = 0;
  // Ralph: const auto &mesh
  for (auto &mesh : meshes) {
    cnt += mesh.getPhotons();
  }
  return cnt;
}

int Model::getTotalFaces() {
  int cnt = 0;
  // Ralph: const auto &mesh
  for (auto &mesh : meshes) {
    cnt += mesh.faces.size();
  }
  return cnt;
}

bool Model::IsInTriangle(const Face &face, const _462::Vector3 &p) {
  _462::Vector3 v0 = vertices[face.vertex3.vi] - vertices[face.vertex1.vi];
  _462::Vector3 v1 = vertices[face.vertex2.vi] - vertices[face.vertex1.vi];
  _462::Vector3 v2 = p - vertices[face.vertex1.vi];
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
  // Ralph: const auto&
  for (auto &mesh : meshes) {
    // Ralph: const auto&
    for (auto &face : mesh.faces) {
      _462::Vector3 intersect = GetIntersect(face, pos, dir);
      if (IsInTriangle(face, intersect)) {
        if (_462::distance(pos, intersect) < distance) {
          min_face = &face;
          min_mesh = &mesh;
          distance = _462::distance(pos, intersect);
        }
      }
    }
  }
  *face = min_face;
  *mesh = min_mesh;
  return distance;
}

const _462::Vector3 Model::GetFaceTextureColor(const Face &face,
                                               const Mesh &mesh,
                                               const _462::Vector3 &p) {
  _462::Vector2 texcoord = getTexcoord(face, p - rel_pos, vertices, texcoords);
  const Texture &texture_info = getTextureInfo(mesh.texture_id);
  int x =
      ((int)(texture_info.w * texcoord.x) % texture_info.w + texture_info.w) %
      texture_info.w;
  int y =
      ((int)(texture_info.h * texcoord.y) % texture_info.h + texture_info.h) %
      texture_info.h;
  // Ralph: maybe the "255.0f" should be defined as a constant
  // Ralph: add some comments in .h file showing what returning values are
  return _462::Vector3(
      texture_info.buffer[3 * (x + y * texture_info.w)] / 255.0f,
      texture_info.buffer[3 * (x + y * texture_info.w) + 1] / 255.0f,
      texture_info.buffer[3 * (x + y * texture_info.w) + 2] / 255.0f);
}

_462::Vector3 Model::GetIntersect(const Face &face,
                                  const _462::Vector3 &line_point,
                                  const _462::Vector3 &line_dir) {
  // Ralph: const _462::Vector3 &plane_normal
  _462::Vector3 plane_normal = face.normal;
  _462::real_t d = _462::dot(vertices[face.vertex1.vi] + rel_pos - line_point,
                             plane_normal) /
                   _462::dot(line_dir, plane_normal);
  // Ralph: What does this line do?
  normalize(line_dir);
  return d * line_dir + line_point;
}

// Ralph: This function looks way too long. Refactor it.
void Model::LoadObjModel(const char *filename) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;

  // I/O operation
  {
    std::string base_dir = GetBaseDir(filename);
    // Ralph: I think this should be handled by `GetBaseDir`
    if (base_dir.empty()) {
      base_dir = ".";
    }
    base_dir += PathSeparator;
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                filename, base_dir.c_str());
    if (!warn.empty()) {
      std::cout << "WARN: " << warn << std::endl;
    }
    // Ralph: If error occurs, should it continue to run?
    if (!err.empty()) {
      std::cerr << err << std::endl;
    }
    // Append `default` material
    materials.push_back(tinyobj::material_t());

    // Load diffuse textures
    {
      for (size_t m = 0; m < materials.size(); m++) {
        // Ralph: const tinyobj::material_t &mp = materials[m];
        tinyobj::material_t *mp = &materials[m];

        if (mp->diffuse_texname.length() > 0) {
          // Only load the texture if it is not already loaded
          if (textures.find(mp->diffuse_texname) == textures.end()) {
            GLuint texture_id;
            int w, h;
            int comp;

            // Ralph: const std::string &texture_filename = mp->diffuse_texname;
            std::string texture_filename = mp->diffuse_texname;
            if (!FileExists(texture_filename)) {
              // Append base dir.
              texture_filename = base_dir + mp->diffuse_texname;
              if (!FileExists(texture_filename)) {
                std::cerr << "Unable to find file: " << mp->diffuse_texname
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

            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
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
            textures.insert(std::make_pair(mp->diffuse_texname, texture_id));

            // test
            // only get RGB
            Texture texture(texture_id, w, h, comp);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE,
                          texture.buffer);
            glBindTexture(GL_TEXTURE_2D, 0);
            texture_infos.push_back(std::move(texture));
          }
        }
      }
    }
  }

  // Convert tiny_obj_loader format
  // Ralph: auto it = attrib.vertices.begin()
  for (std::vector<tinyobj::real_t>::iterator it = attrib.vertices.begin();
       it != attrib.vertices.end(); it += 3)
    vertices.push_back(_462::Vector3((_462::real_t)*it,
                                     (_462::real_t)*std::next(it),
                                     (_462::real_t)*std::next(it, 2)));
  // Ralph: same as above
  for (std::vector<tinyobj::real_t>::iterator it = attrib.normals.begin();
       it != attrib.normals.end(); it += 3)
    normals.push_back(_462::Vector3((_462::real_t)*it,
                                    (_462::real_t)*std::next(it),
                                    (_462::real_t)*std::next(it, 2)));
  // Ralph: same as above
  // Flip y texture coordinate
  for (std::vector<tinyobj::real_t>::iterator it = attrib.texcoords.begin();
       it != attrib.texcoords.end(); it += 2)
    texcoords.push_back(
        _462::Vector2((_462::real_t)*it, (_462::real_t)*std::next(it)));

  // Load mesh
  for (size_t s = 0; s < shapes.size(); s++) {
    // Ralph: `Mesh mesh;` is enough
    Mesh mesh = Mesh();

    // Check for smoothing group and compute smoothing normals
    // Ralph: smooth_vertex_normals
    std::map<int, _462::Vector3> smoothVertexNormals;
    // Ralph: Remove `== 1`. It looks confusing.
    if (hasSmoothingGroup(shapes[s]) == 1) {
      std::cout << "Compute smoothingNormal for shape [" << s << "]"
                << std::endl;
      computeSmoothingNormals(attrib, shapes[s], smoothVertexNormals);
    }

    for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
      Vertex v1(0, 0, 0), v2(0, 0, 0), v3(0, 0, 0);

      tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
      tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
      tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

      // update material_id for face
      int material_id = shapes[s].mesh.material_ids[f];
      if ((material_id < 0) ||
          (material_id >= static_cast<int>(materials.size()))) {
        // Invaid material ID. Use default material.
        // Default material is added to the last item in `materials`.
        material_id = materials.size() - 1;
      }

      float diffuse[3];
      for (size_t i = 0; i < 3; i++) {
        diffuse[i] = materials[material_id].diffuse[i];
      }

      // update texcoord index
      if (attrib.texcoords.size() > 0) {
        if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
            (idx2.texcoord_index < 0)) {
          // face does not contain valid uv index.
          // negative texture coordinate index points to 0
          v1.vti = -1;
          v2.vti = -1;
          v3.vti = -1;
        } else {
          // Don't forget to flip Y coord. for OpenGL rendering
          v1.vti = idx0.texcoord_index;
          v2.vti = idx1.texcoord_index;
          v3.vti = idx2.texcoord_index;
        }
      } else {
        // negative texture coordinate index points to 0
        v1.vti = -1;
        v2.vti = -1;
        v3.vti = -1;
      }

      // update vertex index
      v1.vi = idx0.vertex_index;
      v2.vi = idx1.vertex_index;
      v3.vi = idx2.vertex_index;

      // compute geometric normal
      float v[3][3];  // coordinates
      float n[3][3];  // normals
      for (int k = 0; k < 3; k++) {
        v[0][k] = attrib.vertices[3 * v1.vi + k];
        v[1][k] = attrib.vertices[3 * v2.vi + k];
        v[2][k] = attrib.vertices[3 * v3.vi + k];
      }
      CalcNormal(n[0], v[0], v[1], v[2]);

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
            v1.vni = nf0;
            v2.vni = nf1;
            v3.vni = nf2;
          }
        } else {
          invalid_normal_index = true;
        }

        if (invalid_normal_index && !smoothVertexNormals.empty()) {
          // Use smoothing normals
          int f0 = idx0.vertex_index;
          int f1 = idx1.vertex_index;
          int f2 = idx2.vertex_index;

          if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
            v1.vni = normals.size();
            normals.push_back(smoothVertexNormals[f0]);
            v2.vni = normals.size();
            normals.push_back(smoothVertexNormals[f1]);
            v3.vni = normals.size();
            normals.push_back(smoothVertexNormals[f2]);

            invalid_normal_index = false;
          }
        }

        if (invalid_normal_index) {
          v1.vni = v2.vni = v3.vni = normals.size();
          normals.push_back(_462::Vector3(n[0]));
        }
      }

      mesh.addFace(Face(v1, v2, v3, _462::Vector3(n[0]), material_id));
    }

    // update material_id for mesh
    // OpenGL viewer does not support texturing with per-face material.
    if (shapes[s].mesh.material_ids.size() > 0 &&
        shapes[s].mesh.material_ids.size() > s) {
      mesh.material_id = shapes[s].mesh.material_ids[0];  // use the material ID
                                                          // of the first face.
    } else {
      mesh.material_id = materials.size() - 1;  // = ID for default material.
    }

    // update texture_id
    if ((mesh.material_id < materials.size())) {
      std::string diffuse_texname = materials[mesh.material_id].diffuse_texname;
      if (textures.find(diffuse_texname) != textures.end()) {
        mesh.texture_id = textures[diffuse_texname];
      }
    } else {
      mesh.texture_id = -1;
      std::cout << "Texture for " << filename << " not specified." << std::endl;
    }

    // update mesh
    meshes.push_back(mesh);
  }
}

void Model::render() {
  for (auto &mesh : meshes) {
    mesh.render(materials, rel_pos);
  }
}

void Model::writeBuffer() {
  for (auto &mesh : meshes) {
    mesh.writeOpenGLBuffer(vertices, normals, texcoords);
  }
}

void Model::deleteBuffer() {
  for (auto &mesh : meshes) {
    mesh.deleteOpenGLBuffer();
  }
}

const Texture &Model::getTextureInfo(const GLuint &texture_id) {
  for (const auto &texture_info : texture_infos) {
    if (texture_info.texture_id == texture_id) {
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
    return filepath.substr(0, filepath.find_last_of("/\\"));
  return "";
}

static bool hasSmoothingGroup(const tinyobj::shape_t &shape) {
  // Ralph: Use range-based for loop
  for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) {
    if (shape.mesh.smoothing_group_ids[i] > 0) {
      return true;
    }
  }
  return false;
}

static void computeSmoothingNormals(
    const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
    std::map<int, _462::Vector3> &smoothVertexNormals) {
  smoothVertexNormals.clear();
  std::map<int, _462::Vector3>::iterator iter;

  for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
    // Get the three indexes of the face (all faces are triangular)
    tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
    tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
    tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

    // Get the three vertex indexes and coordinates
    int vi[3];      // indexes
    float v[3][3];  // coordinates

    for (int k = 0; k < 3; k++) {
      vi[0] = idx0.vertex_index;
      vi[1] = idx1.vertex_index;
      vi[2] = idx2.vertex_index;

      v[0][k] = attrib.vertices[3 * vi[0] + k];
      v[1][k] = attrib.vertices[3 * vi[1] + k];
      v[2][k] = attrib.vertices[3 * vi[2] + k];
    }

    // Compute the normal of the face
    float normal[3];
    CalcNormal(normal, v[0], v[1], v[2]);

    // Add the normal to the three vertexes
    for (size_t i = 0; i < 3; ++i) {
      iter = smoothVertexNormals.find(vi[i]);
      if (iter != smoothVertexNormals.end()) {
        // add
        iter->second.x += normal[0];
        iter->second.y += normal[1];
        iter->second.z += normal[2];
      } else {
        smoothVertexNormals[vi[i]].x = normal[0];
        smoothVertexNormals[vi[i]].y = normal[1];
        smoothVertexNormals[vi[i]].z = normal[2];
      }
    }

  }  // f

  // Normalize the normals, that is, make them unit vectors
  for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end();
       iter++) {
    normalize(iter->second);
  }

}  // computeSmoothingNormals

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

}  // namespace photonsimulator

}  // namespace simulator
