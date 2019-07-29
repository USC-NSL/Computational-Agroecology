#include "mesh.h"
#include "../photon_simulator_config.h"
#include "../stdafx.h"

namespace simulator {

namespace photonsimulator {

size_t Mesh::GetPhotons() const {
  size_t cnt = 0;
  for (const auto &face : faces_) {
    cnt += face.photons;
  }
  return cnt;
};

void Mesh::Render(const std::vector<tinyobj::material_t> &materials_,
                  const _462::Vector3 &rel_pos_) {
  GLfloat mat_ambient[] = {materials_[material_id_].ambient[0],
                           materials_[material_id_].ambient[1],
                           materials_[material_id_].ambient[2], 1.0f};
  GLfloat mat_diffuse[] = {materials_[material_id_].diffuse[0],
                           materials_[material_id_].diffuse[1],
                           materials_[material_id_].diffuse[2], 1.0f};
  GLfloat mat_specular[] = {materials_[material_id_].specular[0],
                            materials_[material_id_].specular[1],
                            materials_[material_id_].specular[2], 1.0f};
  GLfloat mat_shininess[] = {materials_[material_id_].shininess};
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glTranslatef(rel_pos_.x, rel_pos_.y, rel_pos_.z);

  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_FILL);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);
  GLsizei stride = (3 + 3 + 2) * sizeof(float);

  if (vb_id_ > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, vb_id_);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // bind texture if loaded
    if (texture_id_ != -1)
      glBindTexture(GL_TEXTURE_2D, texture_id_);
    else {
      glBindTexture(GL_TEXTURE_2D, 0);
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
    }
    glVertexPointer(3, GL_FLOAT, stride, (const void *)0);
    // Ralph: is it correct to have these manual pointers?
    // wym: these are the address indicators.
    glNormalPointer(GL_FLOAT, stride, (const void *)(sizeof(float) * 3));
    glTexCoordPointer(2, GL_FLOAT, stride, (const void *)(sizeof(float) * 6));

    glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles_);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  glPopMatrix();
}

void Mesh::WriteOpenGLBuffer(const std::vector<_462::Vector3> &vertices_,
                             const std::vector<_462::Vector3> &normals_,
                             const std::vector<_462::Vector2> &texcoords_) {
  std::vector<float> buffer;  // 3:vtx, 3:normal, 3:col, 2:texcoord

  for (const auto &face : faces_) {
    buffer.push_back((float)vertices_[face.vertex1.vertex_index].x);
    buffer.push_back((float)vertices_[face.vertex1.vertex_index].y);
    buffer.push_back((float)vertices_[face.vertex1.vertex_index].z);
    buffer.push_back((float)normals_[face.vertex1.normal_index].x);
    buffer.push_back((float)normals_[face.vertex1.normal_index].y);
    buffer.push_back((float)normals_[face.vertex1.normal_index].z);
    if (face.vertex1.texcoord_index != -1) {
      buffer.push_back((float)texcoords_[face.vertex1.texcoord_index].x);
      buffer.push_back((float)texcoords_[face.vertex1.texcoord_index].y);
    } else {
      buffer.push_back((float)0.0);
      buffer.push_back((float)0.0);
    }

    buffer.push_back((float)vertices_[face.vertex2.vertex_index].x);
    buffer.push_back((float)vertices_[face.vertex2.vertex_index].y);
    buffer.push_back((float)vertices_[face.vertex2.vertex_index].z);
    buffer.push_back((float)normals_[face.vertex2.normal_index].x);
    buffer.push_back((float)normals_[face.vertex2.normal_index].y);
    buffer.push_back((float)normals_[face.vertex2.normal_index].z);
    if (face.vertex2.texcoord_index != -1) {
      buffer.push_back((float)texcoords_[face.vertex2.texcoord_index].x);
      buffer.push_back((float)texcoords_[face.vertex2.texcoord_index].y);
    } else {
      buffer.push_back((float)0.0);
      buffer.push_back((float)0.0);
    }

    buffer.push_back((float)vertices_[face.vertex3.vertex_index].x);
    buffer.push_back((float)vertices_[face.vertex3.vertex_index].y);
    buffer.push_back((float)vertices_[face.vertex3.vertex_index].z);
    buffer.push_back((float)normals_[face.vertex3.normal_index].x);
    buffer.push_back((float)normals_[face.vertex3.normal_index].y);
    buffer.push_back((float)normals_[face.vertex3.normal_index].z);
    if (face.vertex3.texcoord_index != -1) {
      buffer.push_back((float)texcoords_[face.vertex3.texcoord_index].x);
      buffer.push_back((float)texcoords_[face.vertex3.texcoord_index].y);
    } else {
      buffer.push_back((float)0.0);
      buffer.push_back((float)0.0);
    }
  }
  vb_id_ = 0;

  if (buffer.size() > 0) {
    glGenBuffers(1, &vb_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vb_id_);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer.at(0),
                 GL_STATIC_DRAW);
    num_triangles_ =
        buffer.size() /
        (kSizeOfVertexBuffer + kSizeOfNormalBuffer + kSizeOfTexcoordBuffer) / 3;
  }
}

// TODO: check if necessary to free buffer manually
void Mesh::DeleteOpenGLBuffer() {}

}  // namespace photonsimulator

}  // namespace simulator
