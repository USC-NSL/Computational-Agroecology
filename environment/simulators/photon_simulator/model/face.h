#ifndef __FACE_H__
#define __FACE_H__
#include "../vectors.h"
#include "../stdafx.h"

#ifndef __MATERIAL_H__
#include "material.h"
#endif

namespace simulator
{

namespace photonsimulator
{

class Vertex
{
public:
  int vi;  // vertex index
  int vni; // normal index
  int vti; // texture coordinate index

  Vertex() = delete;
  Vertex(int vi_, int vni_, int vti_) : vi(vi_), vni(vni_), vti(vti_) {}
  ~Vertex() {}
};

class Face
{
public:
  // keep everything public for simplicity
  Vertex vertex1, vertex2, vertex3;
  Vector3 normal; // face normal
  int material_id;
  Material material; // TODO: how to import material info
  int photons;

  Face() = delete;
  Face(Vertex v1, Vertex v2, Vertex v3, Vector3 normal, int material_id_ = 0)
      : vertex1(v1), vertex2(v2), vertex3(v3), photons(0),
        material_id(material_id_), material(), normal(normal) {}
  ~Face() {}

  // given point on face, return its texture coordinate
  Vector2 getTexcoord(Vector3 pos, const std::vector<Vector3> &vertices,
                      const std::vector<Vector2> &texcoords);

private:
};

} // namespace photonsimulator

} // namespace simulator

#endif /* __FACE_H__ */