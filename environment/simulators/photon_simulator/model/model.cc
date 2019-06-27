#include "model.h"
#include "mesh.h"
#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
#endif
#include <cmath>

namespace simulator
{

namespace photonsimulator
{

const char PathSeparator =
#if defined _WIN32 || defined __CYGWIN__
    '\\';
#else
    '/';
#endif

Model::~Model()
{
  for (auto &texture_info : texture_infos)
  {
    delete texture_info.texture;
  }
  texture_infos.clear();
  textures.clear();
  meshes.clear();
  materials.clear();
  std::cout << "model destroyed." << std::endl;
};

int Model::getPhotons()
{
  int cnt = 0;
  for (auto &mesh : meshes)
  {
    cnt += mesh.getPhotons();
  }
  return cnt;
}

void Model::LoadObjModel(const char *filename)
{
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;

  // I/O operation
  {
    std::string base_dir = GetBaseDir(filename);
    if (base_dir.empty())
    {
      base_dir = ".";
    }
    base_dir += PathSeparator;
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, base_dir.c_str());
    if (!warn.empty())
    {
      std::cout << "WARN: " << warn << std::endl;
    }
    if (!err.empty())
    {
      std::cerr << err << std::endl;
    }
    // Append `default` material
    materials.push_back(tinyobj::material_t());

    // Load diffuse textures
    {
      for (size_t m = 0; m < materials.size(); m++)
      {
        tinyobj::material_t *mp = &materials[m];

        if (mp->diffuse_texname.length() > 0)
        {
          // Only load the texture if it is not already loaded
          if (textures.find(mp->diffuse_texname) == textures.end())
          {
            GLuint texture_id;
            int w, h;
            int comp;

            std::string texture_filename = mp->diffuse_texname;
            if (!FileExists(texture_filename))
            {
              // Append base dir.
              texture_filename = base_dir + mp->diffuse_texname;
              if (!FileExists(texture_filename))
              {
                std::cerr << "Unable to find file: " << mp->diffuse_texname
                          << std::endl;
                exit(1);
              }
            }

            unsigned char *image = stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
            if (!image)
            {
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
            if (comp == 3)
            {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                           GL_UNSIGNED_BYTE, image);
            }
            else if (comp == 4)
            {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                           GL_UNSIGNED_BYTE, image);
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(image);
            textures.insert(std::make_pair(mp->diffuse_texname, texture_id));

            // test
            // only get RGB
            unsigned char *texture = new unsigned char[w * h * 3];
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
            glBindTexture(GL_TEXTURE_2D, 0);
            // for (int i = 500; i < w; i++)
            // {
            //     for (int j = 0; j < h; j++)
            //     {
            //         std::cout << (int)texture[(j + i * h) * 3 + 0] << " "
            //                   << (int)texture[(j + i * h) * 3 + 1] << " "
            //                   << (int)texture[(j + i * h) * 3 + 2] << std::endl;
            //     }
            // }
            std::cout << (int)texture[3 * (316 + 352 * w)] << " " << (int)texture[3 * (316 + 352 * w) + 1] << " " << (int)texture[3 * (316 + 352 * w) + 2] << std::endl;

            texture_infos.push_back(Texture(texture_id, texture, w, h, comp));
          }
        }
      }
    }
  }

  // convert tiny_obj_loader format
  for (std::vector<real_t>::iterator it = attrib.vertices.begin(); it != attrib.vertices.end(); it += 3)
    vertices.push_back(Vector3(*it, *std::next(it), *std::next(it, 2)));
  for (std::vector<real_t>::iterator it = attrib.normals.begin(); it != attrib.normals.end(); it += 3)
    normals.push_back(Vector3(*it, *std::next(it), *std::next(it, 2)));
  for (std::vector<real_t>::iterator it = attrib.texcoords.begin(); it != attrib.texcoords.end(); it += 2)
    texcoords.push_back(Vector2(*it, *std::next(it)));

  for (size_t s = 0; s < shapes.size(); s++)
  {
    Mesh mesh = Mesh();

    // Check for smoothing group and compute smoothing normals
    std::map<int, Vector3> smoothVertexNormals;
    if (hasSmoothingGroup(shapes[s]) == 1)
    {
      std::cout << "Compute smoothingNormal for shape [" << s << "]" << std::endl;
      computeSmoothingNormals(attrib, shapes[s], smoothVertexNormals);
    }

    for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++)
    {

      Vertex v1(0, 0, 0), v2(0, 0, 0), v3(0, 0, 0);

      tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
      tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
      tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

      // update material_id for face
      int material_id = shapes[s].mesh.material_ids[f];
      if ((material_id < 0) ||
          (material_id >= static_cast<int>(materials.size())))
      {
        // Invaid material ID. Use default material.
        // Default material is added to the last item in `materials`.
        material_id = materials.size() - 1;
      }

      float diffuse[3];
      for (size_t i = 0; i < 3; i++)
      {
        diffuse[i] = materials[material_id].diffuse[i];
      }

      // update texcoord index
      if (attrib.texcoords.size() > 0)
      {
        if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
            (idx2.texcoord_index < 0))
        {
          // face does not contain valid uv index.
          // negative texture coordinate index points to 0
          v1.vti = -1;
          v2.vti = -1;
          v3.vti = -1;
        }
        else
        {
          // Don't forget to flip Y coord. for OpenGL rendering
          v1.vti = idx0.texcoord_index;
          v2.vti = idx1.texcoord_index;
          v3.vti = idx2.texcoord_index;
        }
      }
      else
      {
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
      float v[3][3]; // coordinates
      float n[3][3]; // normals
      for (int k = 0; k < 3; k++)
      {
        v[0][k] = attrib.vertices[3 * v1.vi + k];
        v[1][k] = attrib.vertices[3 * v2.vi + k];
        v[2][k] = attrib.vertices[3 * v3.vi + k];
      }
      CalcNormal(n[0], v[0], v[1], v[2]);

      // update normal index
      {
        bool invalid_normal_index = false;
        if (attrib.normals.size() > 0)
        {
          int nf0 = idx0.normal_index;
          int nf1 = idx1.normal_index;
          int nf2 = idx2.normal_index;

          if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0))
          {
            // normal index is missing from this face.
            invalid_normal_index = true;
          }
          else
          {
            v1.vni = nf0;
            v2.vni = nf1;
            v3.vni = nf2;
          }
        }
        else
        {
          invalid_normal_index = true;
        }

        if (invalid_normal_index && !smoothVertexNormals.empty())
        {
          // Use smoothing normals
          int f0 = idx0.vertex_index;
          int f1 = idx1.vertex_index;
          int f2 = idx2.vertex_index;

          if (f0 >= 0 && f1 >= 0 && f2 >= 0)
          {
            v1.vni = normals.size();
            normals.push_back(smoothVertexNormals[f0]);
            v2.vni = normals.size();
            normals.push_back(smoothVertexNormals[f1]);
            v3.vni = normals.size();
            normals.push_back(smoothVertexNormals[f2]);

            invalid_normal_index = false;
          }
        }

        if (invalid_normal_index)
        {
          v1.vni = v2.vni = v3.vni = normals.size();
          normals.push_back(Vector3(n[0]));
        }
      }

      mesh.addFace(Face(v1, v2, v3, Vector3(n[0]), material_id));
    }

    // update material_id for mesh
    // OpenGL viewer does not support texturing with per-face material.
    if (shapes[s].mesh.material_ids.size() > 0 &&
        shapes[s].mesh.material_ids.size() > s)
    {
      mesh.material_id = shapes[s].mesh.material_ids[0]; // use the material ID
                                                         // of the first face.
    }
    else
    {
      mesh.material_id = materials.size() - 1; // = ID for default material.
    }

    // update texture_id
    std::string diffuse_texname = materials[mesh.material_id].diffuse_texname;
    if ((mesh.material_id < materials.size()))
    {
      if (textures.find(diffuse_texname) != textures.end())
      {
        mesh.texture_id = textures[diffuse_texname];
      }
    }
    else
    {
      mesh.texture_id = -1;
      std::cout << "texture " << diffuse_texname << " not found." << std::endl;
    }

    // update mesh
    meshes.push_back(mesh);
  }
}

void Model::render()
{
  for (auto &mesh : meshes)
  {
    mesh.render(materials, rel_pos);
  }
}

void Model::writeBuffer()
{
  for (auto &mesh : meshes)
  {
    mesh.writeOpenGLBuffer(vertices, normals, texcoords);
  }
}

void Model::deleteBuffer()
{
  for (auto &mesh : meshes)
  {
    mesh.deleteOpenGLBuffer();
  }
}

// auxiliary functions

static bool FileExists(const std::string &abs_filename)
{
  bool ret;
  FILE *fp = fopen(abs_filename.c_str(), "rb");
  if (fp)
  {
    ret = true;
    fclose(fp);
  }
  else
  {
    ret = false;
  }

  return ret;
}

static std::string GetBaseDir(const std::string &filepath)
{
  if (filepath.find_last_of("/\\") != std::string::npos)
    return filepath.substr(0, filepath.find_last_of("/\\"));
  return "";
}

static bool hasSmoothingGroup(const tinyobj::shape_t &shape)
{
  for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++)
  {
    if (shape.mesh.smoothing_group_ids[i] > 0)
    {
      return true;
    }
  }
  return false;
}

static void computeSmoothingNormals(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape, std::map<int, Vector3> &smoothVertexNormals)
{
  smoothVertexNormals.clear();
  std::map<int, Vector3>::iterator iter;

  for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++)
  {
    // Get the three indexes of the face (all faces are triangular)
    tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
    tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
    tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

    // Get the three vertex indexes and coordinates
    int vi[3];     // indexes
    float v[3][3]; // coordinates

    for (int k = 0; k < 3; k++)
    {
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
    for (size_t i = 0; i < 3; ++i)
    {
      iter = smoothVertexNormals.find(vi[i]);
      if (iter != smoothVertexNormals.end())
      {
        // add
        iter->second.x += normal[0];
        iter->second.y += normal[1];
        iter->second.z += normal[2];
      }
      else
      {
        smoothVertexNormals[vi[i]].x = normal[0];
        smoothVertexNormals[vi[i]].y = normal[1];
        smoothVertexNormals[vi[i]].z = normal[2];
      }
    }

  } // f

  // Normalize the normals, that is, make them unit vectors
  for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end();
       iter++)
  {
    normalize(iter->second);
  }

} // computeSmoothingNormals

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3])
{
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
  if (len2 > 0.0f)
  {
    float len = sqrtf(len2);

    N[0] /= len;
    N[1] /= len;
    N[2] /= len;
  }
}

} // namespace photonsimulator

} // namespace simulator
