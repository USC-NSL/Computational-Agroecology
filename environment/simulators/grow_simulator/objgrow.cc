#include <fstream>

#include "objgrow.h"

namespace simulator {

namespace growsimulator {

ObjGrow::ObjGrow(
  const double rightHorizontalDegree, const double rightVerticalDegree, const double rightGrowLengthRatio,
  const double leftHorizontalDegree, const double leftVerticalDegree, const double leftGrowLengthRatio,
  const double length)
	: rightHorizontalDegree_(DegreeToRadian(rightHorizontalDegree)), 
      rightVerticalDegree_(DegreeToRadian(90 - rightVerticalDegree)),
      rightGrowLengthRatio_(rightGrowLengthRatio),
      leftHorizontalDegree_(DegreeToRadian(leftHorizontalDegree)),
      leftVerticalDegree_(DegreeToRadian(90 - leftVerticalDegree)),
      leftGrowLengthRatio_(leftGrowLengthRatio) {
  plantRootsToGrow.push_back(PlantRoot(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(1, 0, 0), length));
}

double ObjGrow::DegreeToRadian(const double degree) const {
  return degree * kPI / 180.0;
}

double ObjGrow::RadianToDegree(const double radian) const {
  return radian / kPI * 180.0;
}

void ObjGrow::PlantGrow() {
  auto plantrootEnd = plantRootsToGrow.end();
  for (auto plantroot = plantRootsToGrow.begin(); plantroot != plantrootEnd;) {
    Vector3 right = getRightDir(*plantroot, rightHorizontalDegree_, rightVerticalDegree_);
    plantRootsToGrow.push_front(PlantRoot(plantroot->pos + plantroot->length * plantroot->dir,
      right,
      normalize(cross(right, plantroot->norm)),
      rightGrowLengthRatio_ * plantroot->length));
    Vector3 left = getLeftDir(*plantroot, leftHorizontalDegree_, leftVerticalDegree_);
    plantRootsToGrow.push_front(PlantRoot(plantroot->pos + plantroot->length * plantroot->dir,
      left,
      normalize(cross(left, plantroot->norm)),
      leftGrowLengthRatio_ * plantroot->length));
    plantRootsHasGrown.push_back(*plantroot);
    /*std::cout << "-------------------------------" << std::endl;
    std::cout << RadianToDegree(getDegree(plantroot->dir, right)) << std::endl;
    std::cout << RadianToDegree(getDegree(plantroot->norm, right)) << std::endl;
    std::cout << RadianToDegree(getDegree(plantroot->dir, left)) << std::endl;
    std::cout << RadianToDegree(getDegree(plantroot->norm, left)) << std::endl;
    std::cout << "-------------------------------" << std::endl;*/
    plantroot = plantRootsToGrow.erase(plantroot);
  }
}

void ObjGrow::PlantGrow(
  const double rightHorizontalDegree, const double rightVerticalDegree, const double rightGrowLengthRatio,
  const double leftHorizontalDegree, const double leftVerticalDegree, const double leftGrowLengthRatio,
  const double length) {
  auto plantrootEnd = plantRootsToGrow.end();
  for (auto plantroot = plantRootsToGrow.begin(); plantroot != plantrootEnd;) {
    Vector3 right = getRightDir(*plantroot, rightHorizontalDegree, rightVerticalDegree);
    plantRootsToGrow.push_front(PlantRoot(plantroot->pos + plantroot->length * plantroot->dir,
      right,
      normalize(cross(right, plantroot->norm)),
      rightGrowLengthRatio * plantroot->length));
    Vector3 left = getLeftDir(*plantroot, leftHorizontalDegree, leftVerticalDegree);
    plantRootsToGrow.push_front(PlantRoot(plantroot->pos + plantroot->length * plantroot->dir,
      left,
      normalize(cross(left, plantroot->norm)),
      leftGrowLengthRatio * plantroot->length));
    plantRootsHasGrown.push_back(*plantroot);
    /*std::cout << "-------------------------------" << std::endl;
    std::cout << RadianToDegree(getDegree(plantroot->dir, right)) << std::endl;
    std::cout << RadianToDegree(getDegree(plantroot->norm, right)) << std::endl;
    std::cout << RadianToDegree(getDegree(plantroot->dir, left)) << std::endl;
    std::cout << RadianToDegree(getDegree(plantroot->norm, left)) << std::endl;
    std::cout << "-------------------------------" << std::endl;*/
    plantroot = plantRootsToGrow.erase(plantroot);
  }
}

std::list<int> ObjGrow::getListByInt(int a, int b, int c) {
  std::list<int> f;
  f.push_back(a);
  f.push_back(b);
  f.push_back(c);
  return f;
}

void ObjGrow::ExportObj(const char* pathname, double width) {
  std::ofstream file(pathname);
  std::list<Vector3> v;
  std::list<std::list<int>> f;
  int index = 0;
  for (auto plantroot : plantRootsHasGrown) {
    Vector3 rightDir = cross(plantroot.dir, plantroot.norm);
    v.push_back(plantroot.pos + width / 2 * plantroot.norm + width / 2 * rightDir);
    v.push_back(plantroot.pos - width / 2 * plantroot.norm + width / 2 * rightDir);
    v.push_back(plantroot.pos + width / 2 * plantroot.norm - width / 2 * rightDir);
    v.push_back(plantroot.pos - width / 2 * plantroot.norm - width / 2 * rightDir);
    v.push_back(plantroot.pos + plantroot.dir * plantroot.length + width / 2 * plantroot.norm + width / 2 * rightDir);
    v.push_back(plantroot.pos + plantroot.dir * plantroot.length - width / 2 * plantroot.norm + width / 2 * rightDir);
    v.push_back(plantroot.pos + plantroot.dir * plantroot.length + width / 2 * plantroot.norm - width / 2 * rightDir);
    v.push_back(plantroot.pos + plantroot.dir * plantroot.length - width / 2 * plantroot.norm - width / 2 * rightDir);
    f.push_back(getListByInt(index * 8 + 1, index * 8 + 2, index * 8 + 3));
    f.push_back(getListByInt(index * 8 + 4, index * 8 + 2, index * 8 + 3));

    f.push_back(getListByInt(index * 8 + 1, index * 8 + 2, index * 8 + 5));
    f.push_back(getListByInt(index * 8 + 6, index * 8 + 2, index * 8 + 5));

    f.push_back(getListByInt(index * 8 + 1, index * 8 + 3, index * 8 + 5));
    f.push_back(getListByInt(index * 8 + 7, index * 8 + 3, index * 8 + 5));

    f.push_back(getListByInt(index * 8 + 3, index * 8 + 4, index * 8 + 7));
    f.push_back(getListByInt(index * 8 + 8, index * 8 + 4, index * 8 + 7));

    f.push_back(getListByInt(index * 8 + 5, index * 8 + 6, index * 8 + 7));
    f.push_back(getListByInt(index * 8 + 8, index * 8 + 6, index * 8 + 7));

    f.push_back(getListByInt(index * 8 + 2, index * 8 + 4, index * 8 + 6));
    f.push_back(getListByInt(index * 8 + 8, index * 8 + 4, index * 8 + 6));
    index += 1;
  }
  for (auto vitem : v) {
    file << "v " << vitem.x << " " << vitem.y << " " << vitem.z << std::endl;
  }
  file << std::endl;
  for (auto fitem : f) {
    file << "f " << fitem.front() << " ";
    fitem.pop_front();
    file << fitem.front() << " ";
    fitem.pop_front();
    file << fitem.front() << std::endl;
    fitem.pop_front();
  }
  file.close();
}

real_t ObjGrow::getDegree(const Vector3 & left, const Vector3 & right) {
  real_t res = dotresult(left, right);
  if (length(left) < kMin || length(right) < kMin)
    return 0.0;
  return acos(res / length(left) / length(right));
}

Vector3 ObjGrow::getLeftDir(const PlantRoot & plantroot, double HorizontalDegree, double VerticalDegree) {
  Vector3 RightNorm = normalize(cross(plantroot.dir, plantroot.norm)), dirForRoot;
  if (fabs(VerticalDegree - kPI / 2) < kMin)
    dirForRoot = plantroot.dir;
  else dirForRoot = cos(HorizontalDegree) * plantroot.norm - sin(HorizontalDegree) * RightNorm + tan(VerticalDegree) * plantroot.dir;
  return normalize(dirForRoot);
}

Vector3 ObjGrow::getRightDir(const PlantRoot & plantroot, double HorizontalDegree, double VerticalDegree) {
  Vector3 RightNorm = normalize(cross(plantroot.dir, plantroot.norm)), dirForRoot;
  if (fabs(VerticalDegree - kPI / 2) < kMin)
    dirForRoot = plantroot.dir;
  else dirForRoot = cos(HorizontalDegree) * plantroot.norm + sin(HorizontalDegree) * RightNorm + tan(VerticalDegree) * plantroot.dir;
  return normalize(dirForRoot);
}

void ObjGrow::getMultiRes(real_t * left, real_t * right, real_t * res, const int m, const int n, const int t) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < t; j++) {
      res[i * t + j] = 0;
      for (int k = 0; k < n; k++) {
        res[i * t + j] += left[i * n + k] * right[k * t + j];
      }
    }
  }
}

}  // namespace growsimulator

}  // namespace simulator