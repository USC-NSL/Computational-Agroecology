/* 
    the class is a wrapper for mesh
    in order to load file and render in GL
*/

#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "common/stdafx.h"
#include "model/mesh.h"

class Texture
{
public:
    GLuint texture_id;
    unsigned char *texture;
    int w, h;
    int comp; // 3 = rgb, 4 = rgba
    Texture() = delete;
    Texture(GLuint texture_id_, unsigned char *texture_, int w_, int h_, int comp_)
        : texture_id(texture_id_), texture(texture_), w(w_), h(h_), comp(comp_) {}
};

class Model
{
public:
    // keep everything public for simplicity
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;
    std::vector<Mesh> meshes;
    std::map<std::string, GLuint> textures;
    std::vector<tinyobj::material_t> materials;
    std::vector<Texture> texture_infos;

    Vector3 rel_pos;
    void setRelativePos(Vector3 pos) { rel_pos = pos; }

    Model() = delete;
    void LoadObjModel(const char *filename);
    int size() { return meshes.size(); }
    Mesh &operator[](int index) { return meshes[index]; }
    Model(const char *filename, Vector3 pos = Vector3(0.0, 0.0, 0.0)) : rel_pos(pos) { LoadObjModel(filename); }
    ~Model();
    int getPhotons();

    // add to buffer for OpenGL rendering
    void writeBuffer();
    void deleteBuffer();
    void render();
    Texture getTextureInfo(GLuint texture_id)
    {
        for (auto &texture_info : texture_infos)
        {
            if (texture_info.texture_id == texture_id)
                return texture_info;
        }
        assert(0);
    }

private:
};

// auxiliary functions
static bool FileExists(const std::string &abs_filename);
static std::string GetBaseDir(const std::string &filepath);
static bool hasSmoothingGroup(const tinyobj::shape_t &shape);
static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);
static void computeSmoothingNormals(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape, std::map<int, Vector3> &smoothVertexNormals);

#endif