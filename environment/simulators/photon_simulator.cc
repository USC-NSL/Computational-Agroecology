#include "photon_simulator.h"

#include "environment.h"

// extend third party library
namespace _462 {
/**
 * Returns the Hadamard product of two vectors
 */
inline Vector3 Hadamard(const Vector3 &lhs, const Vector3 &rhs) {
  return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
}  // namespace _462

namespace simulator {

namespace photonsimulator {

PhotonSimulator::PhotonSimulator(const int number, const _462::real_t distance,
                                 const _462::real_t height)
    : kNumberOfPhotonsNeayby(number),
      kMaxDistance(distance),
      kSunHeight(height) {}

void PhotonSimulator::SimulateToTime(
    environment::Environment *env,
    const std::chrono::system_clock::time_point &time) {
  // the part for model
  FreeModels();
  LoadModels(env);

  // the part for photon
  /**
   * TODO: check terrain orientation
   * we regard north as y asix
   */
  alive_photons.clear();
  absorb_photons.clear();
  PhotonEmit(_462::Vector3(-sin(env->sun_info().SunAzimuth),
                           -cos(env->sun_info().SunAzimuth),
                           -cos(env->sun_info().SolarAltitude)),
             _462::Vector3(env->sun_info().HourlyIrradiance,
                           env->sun_info().HourlyIrradiance,
                           env->sun_info().HourlyIrradiance),
             env->config_.location.latitude_bottom,
             env->config_.location.latitude_top,
             (env->config_.location.latitude_top -
              env->config_.location.latitude_bottom) /
                 100.0f,
             env->config_.location.longitude_left,
             env->config_.location.longitude_right,
             (env->config_.location.longitude_right -
              env->config_.location.longitude_left) /
                 100.0f);
  PhotonsModify();

  // write result to env
  WriteResultToEnv(env);
}

void PhotonSimulator::FreeModels() {
  while (!models.empty()) {
    Model *model = models.back();
    models.pop_back();
    delete model;
  }
}

// TODO: implement these two functions after refining class plant
void PhotonSimulator::LoadModels(environment::Environment *env) {}
void PhotonSimulator::WriteResultToEnv(environment::Environment *env) {}

void PhotonSimulator::PhotonEmit(
    const _462::Vector3 &sun_direction, const _462::Vector3 &sun_strength,
    const double latitude_bottom, const double latitude_top,
    const double latitudeDiff, const double longitude_left,
    const double longitude_right, const double longitudeDiff) {
  for (_462::real_t i = (_462::real_t)latitude_bottom;
       i <= (_462::real_t)latitude_top; i += (_462::real_t)latitudeDiff) {
    for (_462::real_t j = (_462::real_t)longitude_left;
         j <= (_462::real_t)longitude_right; j += (_462::real_t)longitudeDiff) {
      alive_photons.push_back(
          Photon(sun_direction, _462::Vector3(i, j, kSunHeight), sun_strength));
    }
  }
}

void PhotonSimulator::ConstructKDTree(std::vector<Photon> &p,
                                      const unsigned int begin,
                                      const unsigned int end) {
  if (end - begin == 0)
    return;
  if (end - begin == 1) {
    p[begin].flag = kLEAF;
    return;
  }
  unsigned median = begin + (end - begin) / 2;
  _462::real_t x_avg = 0.0, y_avg = 0.0, z_avg = 0.0;
  _462::real_t x_var = 0.0, y_var = 0.0, z_var = 0.0;
  _462::real_t n = (_462::real_t)(end - begin);
  std::vector<Photon>::iterator a = p.begin() + begin;
  std::vector<Photon>::iterator b = p.begin() + end;
  std::vector<Photon>::iterator it;
  for (it = a; it != b; ++it) {
    x_avg += (*it).pos.x;
    y_avg += (*it).pos.y;
    z_avg += (*it).pos.z;
  }
  for (it = a; it != b; ++it) {
    x_var += ((*it).pos.x - x_avg) * ((*it).pos.x - x_avg);
    y_var += ((*it).pos.y - y_avg) * ((*it).pos.y - y_avg);
    z_var += ((*it).pos.z - z_avg) * ((*it).pos.z - z_avg);
  }
  x_var /= n;
  y_var /= n;
  z_var /= n;
  _462::real_t max_var = std::max(std::max(x_var, y_var), z_var);
  if (max_var == x_var) {
    std::sort(p.begin() + begin, p.begin() + end, CompareX);
    p[median].flag = kXAXIS;
  }
  if (max_var == y_var) {
    std::sort(p.begin() + begin, p.begin() + end, CompareY);
    p[median].flag = kYAXIS;
  }
  if (max_var == z_var) {
    std::sort(p.begin() + begin, p.begin() + end, CompareZ);
    p[median].flag = kZAXIS;
  }
  ConstructKDTree(p, begin, median);
  ConstructKDTree(p, median + 1, end);
}

void PhotonSimulator::LookuptKDTree(
    const std::vector<Photon> &p, const _462::Vector3 &point,
    const _462::Vector3 &norm, Neighbor *neighbors, const unsigned int begin,
    const unsigned int end, _462::real_t &distance, int &size) {
  if (begin == end)
    return;
  else if (begin + 1 == end)
    AddNeighbor(p[begin].pos, p[begin].dir, point, norm, neighbors, begin,
                distance, kMaxDistance, size, kNumberOfPhotonsNeayby);
  else {
    unsigned int median = begin + (end - begin) / 2;
    int flag = (p[median]).flag;
    _462::real_t split_value = GetSplitValueByIndex(p, median, flag);
    _462::real_t p_value = GetSplitValueByPhoton(point, flag);
    if (p_value <= split_value) {
      LookuptKDTree(p, point, norm, neighbors, begin, median, distance, size);
      AddNeighbor(p[median].pos, p[median].dir, point, norm, neighbors, median,
                  distance, kMaxDistance, size, kNumberOfPhotonsNeayby);
      if (size < kNumberOfPhotonsNeayby ||
          (p_value - split_value) * (p_value - split_value) < distance)
        LookuptKDTree(p, point, norm, neighbors, median + 1, end, distance,
                      size);
    } else {
      LookuptKDTree(p, point, norm, neighbors, median + 1, end, distance, size);
      AddNeighbor(p[median].pos, p[median].dir, point, norm, neighbors, median,
                  distance, kMaxDistance, size, kNumberOfPhotonsNeayby);
      if (size < kNumberOfPhotonsNeayby ||
          (p_value - split_value) * (p_value - split_value) < distance)
        LookuptKDTree(p, point, norm, neighbors, begin, median, distance, size);
    }
  }
}

_462::Vector3 PhotonSimulator::GetIntersect(const _462::Vector3 &p1,
                                            const _462::Vector3 &p2,
                                            const _462::Vector3 &p3,
                                            const _462::Vector3 &line_point,
                                            const _462::Vector3 &line_dir) {
  _462::Vector3 plane_normal = GetNormal(p1, p2, p3);
  _462::real_t d = _462::dot(p1 - line_point, plane_normal) /
                   _462::dot(line_dir, plane_normal);
  normalize(line_dir);
  return d * line_dir + line_point;
}

bool PhotonSimulator::IsInTriangle(const _462::Vector3 &a,
                                   const _462::Vector3 &b,
                                   const _462::Vector3 &c,
                                   const _462::Vector3 &p) {
  _462::Vector3 v0 = c - a;
  _462::Vector3 v1 = b - a;
  _462::Vector3 v2 = p - a;
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

_462::Vector3 PhotonSimulator::GetNormal(const _462::Vector3 &p1,
                                         const _462::Vector3 &p2,
                                         const _462::Vector3 &p3) {
  _462::real_t a =
      (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
  _462::real_t b =
      (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
  _462::real_t c =
      (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
  return _462::Vector3(a, b, c);
}

_462::Vector3 PhotonSimulator::GetPixelColor(const _462::Vector3 &ray_pos,
                                             const _462::Vector3 &ray_dir) {
  _462::Vector3 direct, global;
  _462::Vector3 p(1000.0f, 1000.0f, 0.0f);
  Face *min = NULL;
  GLuint texture_id;
  Model *min_model;
  _462::Vector3 min_normal;
  for (auto &model : models) {
    for (auto &mesh : model->meshes) {
      for (auto &face : mesh.faces) {
        _462::Vector3 a = model->vertices[face.vertex1.vi] + model->rel_pos;
        _462::Vector3 b = model->vertices[face.vertex2.vi] + model->rel_pos;
        _462::Vector3 c = model->vertices[face.vertex3.vi] + model->rel_pos;
        _462::Vector3 normal = GetNormal(a, b, c);
        _462::Vector3 intersect = GetIntersect(a, b, c, ray_pos, ray_dir);
        if (IsInTriangle(a, b, c, intersect)) {
          if (distance(ray_pos, intersect) < distance(ray_pos, p)) {
            p = intersect;
            min = &face;
            texture_id = mesh.texture_id;
            min_model = model;
            if (_462::dot(normal, ray_dir) > 0.0f) {
              normal *= -1.0;
            }
            min_normal = normal;
          }
        }
      }
    }
  }
  _462::Vector3 result;
  if (min) {
    _462::Vector2 texcoord = min->getTexcoord(
        p - min_model->rel_pos, min_model->vertices, min_model->texcoords);
    Texture texture_info = min_model->getTextureInfo(texture_id);
    int x =
        ((int)(texture_info.w * texcoord.x) % texture_info.w + texture_info.w) %
        texture_info.w;
    int y =
        ((int)(texture_info.h * texcoord.y) % texture_info.h + texture_info.h) %
        texture_info.h;
    direct = _462::Vector3(
        texture_info.texture[3 * (x + y * texture_info.w)] / 255.0f,
        texture_info.texture[3 * (x + y * texture_info.w) + 1] / 255.0f,
        texture_info.texture[3 * (x + y * texture_info.w) + 2] / 255.0f);

    global = _462::Vector3(0.0, 0.0, 0.0);
    int size = 0;
    // TODO: @Hangjie I change float to _462::real_t(double), please check validity
    _462::real_t d = 0.0;
    int count = 0;
    Neighbor neighbors[kNumberOfPhotonsNeayby];
    LookuptKDTree(absorb_photons, p, min_normal, neighbors, 0,
                  absorb_photons.size() - 1, d, size);
    for (int i = 0; i < size; i++) {
      _462::real_t dist = distance(absorb_photons[neighbors[i].i].pos, p);
      _462::Vector3 color = absorb_photons[neighbors[i].i].power;
      if (dist >= 1.0f) {
        color /= dist;
      }
      _462::real_t dr1 = _462::dot(absorb_photons[neighbors[i].i].dir,
                                   absorb_photons[neighbors[i].i].pos - p);
      global += color;
      count++;
    }
    if (count) {
      global /= (_462::real_t)count;
      result = _462::Hadamard(direct, global);
    } else {
      result = _462::Vector3(0.0f, 0.0f, 0.0f);
    }
  } else {
    result = _462::Vector3(0.0f, 0.0f, 0.0f);
  }
  return result;
}

_462::Vector3 PhotonSimulator::GetRayColor(const int x, const int y,
                                           const int scene_length,
                                           const int scene_width,
                                           const _462::Vector3 &camera_pos,
                                           const _462::Vector3 &camera_ctr,
                                           const _462::Vector3 &camera_up) {
  _462::Vector3 dir = GetRayDir(x, y, scene_length, scene_width, camera_pos,
                                camera_ctr, camera_up);
  _462::Vector3 color = GetPixelColor(camera_pos, dir);
  return color;
}

_462::Vector3 PhotonSimulator::GetRayDir(const int x, const int y,
                                         const int scene_length,
                                         const int scene_width,
                                         const _462::Vector3 &camera_pos,
                                         const _462::Vector3 &camera_ctr,
                                         const _462::Vector3 &camera_up) {
  _462::Vector3 dir = camera_ctr - camera_pos;
  _462::Vector3 cR = _462::cross(dir, camera_up);
  _462::Vector3 cU = _462::cross(cR, dir);
  _462::real_t AR = _462::length(cU) / _462::length(cR);
  _462::real_t dist = tan(kPI / 4) * 2 / _462::length(dir);
  _462::Vector3 t =
      dir + dist * (((_462::real_t)y - (_462::real_t)scene_width / 2) /
                        scene_width * cU +
                    AR * ((_462::real_t)x - (_462::real_t)scene_length / 2) /
                        scene_length * cR);
  t = normalize(t);
  return t;
}

int PhotonSimulator::RussianRoulette(const _462::real_t abr,
                                     const _462::real_t ref,
                                     const _462::real_t trans) {
  _462::real_t a = (rand() % 100) / 100.0f;
  if (a < abr)
    return kAbsorb;
  else if (a < abr + ref)
    return kReflect;
  else
    return kTrans;
}

void PhotonSimulator::PhotonsModify() {
  while (!alive_photons.empty()) {
    for (int i = 0; i < alive_photons.size(); i++) {
      _462::Vector3 p(-100.0f, -100.0f, -100.0f);
      Face *min = NULL;
      Model *min_model;
      _462::Vector3 min_normal;
      for (auto &model : models) {
        for (auto &mesh : model->meshes) {
          for (auto &face : mesh.faces) {
            _462::Vector3 a = model->vertices[face.vertex1.vi] + model->rel_pos;
            _462::Vector3 b = model->vertices[face.vertex2.vi] + model->rel_pos;
            _462::Vector3 c = model->vertices[face.vertex3.vi] + model->rel_pos;
            _462::Vector3 normal = GetNormal(a, b, c);
            _462::Vector3 intersect = GetIntersect(
                a, b, c, alive_photons[i].pos, alive_photons[i].dir);
            if (IsInTriangle(a, b, c, intersect)) {
              if (distance(alive_photons[i].pos, intersect) <
                  distance(alive_photons[i].pos, p)) {
                p = intersect;
                min = &face;
                min_model = model;
                if (_462::dot(normal, alive_photons[i].dir) > 0.0f) {
                  normal *= -1.0;
                }
                min_normal = normal;
              }
            }
          }
        }
      }
      if (min) {
        int res =
            RussianRoulette(min->material.aborption, min->material.reflection,
                            min->material.transmision);
        switch (res) {
          case kAbsorb: {
            absorb_photons.push_back(
                Photon(min_normal, p, alive_photons[i].power));
            alive_photons.erase(alive_photons.begin() + i--);
            min->photons++;
            break;
          }
          case kReflect: {
            _462::Vector3 ref = GetReflect(alive_photons[i].dir, min_normal);
            alive_photons[i].pos = p;
            alive_photons[i].dir = ref;
            break;
          }
          case kTrans: {
            _462::Vector3 ref =
                GetRefract(alive_photons[i].dir, min_normal, 1.0);
            alive_photons[i].pos = p;
            alive_photons[i].dir = ref;
            break;
          }
          default:
            std::cout << "Error result!" << std::endl;
        }
      } else {
        alive_photons.erase(alive_photons.begin() + i--);
      }
    }
  }
  ConstructKDTree(absorb_photons, 0, absorb_photons.size());
}

_462::Vector3 PhotonSimulator::GetReflect(const _462::Vector3 &dir,
                                          const _462::Vector3 &norm) {
  _462::Vector3 normal = normalize(norm);
  return (dir - 2 * _462::Hadamard((_462::Hadamard(dir, normal)), normal));
}

_462::Vector3 PhotonSimulator::GetRefract(const _462::Vector3 &dir,
                                          const _462::Vector3 &norm,
                                          _462::real_t coef) {
  return _462::Vector3(
      -sqrt(1 - coef * coef *
                    (1 - _462::squared_length(_462::Hadamard(dir, norm)) *
                             _462::squared_length(_462::Hadamard(dir, norm)))) *
          norm +
      coef * (dir + _462::squared_length(_462::Hadamard(dir, norm)) * norm));
}

}  // namespace photonsimulator

}  // namespace simulator
