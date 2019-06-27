#include "face.h"
#include "vectors.h"
#include <cassert>

namespace simulator
{

namespace photonsimulator
{

Vector2 Face::getTexcoord(Vector3 pos, const std::vector<Vector3> &vertices,
                          const std::vector<Vector2> &texcoords)
{
  Vector3 vertex12, vertex13, vertex1p;
  real_t k12, k13;
  Vector2 texcoord;
  vertex12 = vertices[vertex2.vi] - vertices[vertex1.vi];
  vertex13 = vertices[vertex3.vi] - vertices[vertex1.vi];
  vertex1p = pos - vertices[vertex1.vi];
  k12 = length(cross(vertex13, vertex1p)) / length(cross(vertex12, vertex13));
  k13 = length(cross(vertex12, vertex1p)) / length(cross(vertex12, vertex13));
  texcoord = (k12 + k13) * texcoords[vertex1.vti] +
             (1 - k12) * texcoords[vertex2.vti] +
             (1 - k13) * texcoords[vertex3.vti];
  return texcoord;
}

} // namespace photonsimulator

} // namespace simulator
