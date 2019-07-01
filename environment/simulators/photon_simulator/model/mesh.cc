//???#define _CRT_SECURE_NO_WARNINGS
#include "../stdafx.h"
#include "mesh.h"

namespace simulator
{

namespace photonsimulator
{

int Mesh::getPhotons()
{
  int cnt = 0;
  for (auto &face : faces)
  {
    cnt += face.photons;
  }
  return cnt;
};

void Mesh::render(std::vector<tinyobj::material_t> &materials,
                  Vector3 rel_pos)
{
  GLfloat mat_ambient[] = {materials[material_id].ambient[0], materials[material_id].ambient[1], materials[material_id].ambient[2], 1.0f};
  GLfloat mat_diffuse[] = {materials[material_id].diffuse[0], materials[material_id].diffuse[1], materials[material_id].diffuse[2], 1.0f};
  GLfloat mat_specular[] = {materials[material_id].specular[0], materials[material_id].specular[1], materials[material_id].specular[2], 1.0f};
  GLfloat mat_shininess[] = {materials[material_id].shininess};
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glTranslatef(rel_pos.x, rel_pos.y, rel_pos.z);

  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_FILL);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);
  GLsizei stride = (3 + 3 + 2) * sizeof(float);

  if (vb_id > 0)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vb_id);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // bind texture if loaded
    if (texture_id != -1)
      glBindTexture(GL_TEXTURE_2D, texture_id);
    else
    {
      glBindTexture(GL_TEXTURE_2D, 0);
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
    }
    glVertexPointer(3, GL_FLOAT, stride, (const void *)0);
    glNormalPointer(GL_FLOAT, stride, (const void *)(sizeof(float) * 3));
    glTexCoordPointer(2, GL_FLOAT, stride, (const void *)(sizeof(float) * 6));

    glDrawArrays(GL_TRIANGLES, 0, 3 * numTriangles);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  glPopMatrix();
}

void Mesh::writeOpenGLBuffer(const std::vector<Vector3> &vertices,
                             const std::vector<Vector3> &normals,
                             const std::vector<Vector2> &texcoords)
{
  std::vector<float> buffer; // 3:vtx, 3:normal, 3:col, 2:texcoord

  for (auto &face : faces)
  {
    buffer.push_back((float)vertices[face.vertex1.vi].x);
    buffer.push_back((float)vertices[face.vertex1.vi].y);
    buffer.push_back((float)vertices[face.vertex1.vi].z);
    buffer.push_back((float)normals[face.vertex1.vni].x);
    buffer.push_back((float)normals[face.vertex1.vni].y);
    buffer.push_back((float)normals[face.vertex1.vni].z);
    if (face.vertex1.vti != -1)
    {
      buffer.push_back((float)texcoords[face.vertex1.vti].x);
      buffer.push_back((float)texcoords[face.vertex1.vti].y);
    }
    else
    {
      buffer.push_back((float)0.0);
      buffer.push_back((float)0.0);
    }

    buffer.push_back((float)vertices[face.vertex2.vi].x);
    buffer.push_back((float)vertices[face.vertex2.vi].y);
    buffer.push_back((float)vertices[face.vertex2.vi].z);
    buffer.push_back((float)normals[face.vertex2.vni].x);
    buffer.push_back((float)normals[face.vertex2.vni].y);
    buffer.push_back((float)normals[face.vertex2.vni].z);
    if (face.vertex2.vti != -1)
    {
      buffer.push_back((float)texcoords[face.vertex2.vti].x);
      buffer.push_back((float)texcoords[face.vertex2.vti].y);
    }
    else
    {
      buffer.push_back((float)0.0);
      buffer.push_back((float)0.0);
    }

    buffer.push_back((float)vertices[face.vertex3.vi].x);
    buffer.push_back((float)vertices[face.vertex3.vi].y);
    buffer.push_back((float)vertices[face.vertex3.vi].z);
    buffer.push_back((float)normals[face.vertex3.vni].x);
    buffer.push_back((float)normals[face.vertex3.vni].y);
    buffer.push_back((float)normals[face.vertex3.vni].z);
    if (face.vertex3.vti != -1)
    {
      buffer.push_back((float)texcoords[face.vertex3.vti].x);
      buffer.push_back((float)texcoords[face.vertex3.vti].y);
    }
    else
    {
      buffer.push_back((float)0.0);
      buffer.push_back((float)0.0);
    }
  }
  vb_id = 0;

  int size = (int)buffer.size();
  if (buffer.size() > 0)
  {
    glGenBuffers(1, &vb_id);
    glBindBuffer(GL_ARRAY_BUFFER, vb_id);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
                 &buffer.at(0), GL_STATIC_DRAW);
    numTriangles = buffer.size() / (3 + 3 + 2) / 3; // 3:vtx, 3:normal, 2:texcoord

    // printf("shape[] # of triangles = %d\n", numTriangles);
  }
}

void Mesh::deleteOpenGLBuffer()
{
  glDeleteBuffers(1, &vb_id);
}

} // namespace photonsimulator

} // namespace simulator
