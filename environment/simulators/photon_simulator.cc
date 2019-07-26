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
  // TODO: add functionality if render is required.
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

void PhotonSimulator::LookuptKDTree(const std::vector<Photon> &p,
                                    const _462::Vector3 &point,
                                    const _462::Vector3 &norm,
                                    std::vector<Neighbor> &heap,
                                    const unsigned int begin,
                                    const unsigned int end,
                                    _462::real_t &distance) {
  if (begin == end)
    return;
  else if (begin + 1 == end)
    AddNeighbor(p[begin].pos, p[begin].dir, point, norm, heap, begin, distance,
                kMaxDistance, kNumberOfPhotonsNeayby);
  else {
    unsigned int median = begin + (end - begin) / 2;
    int flag = (p[median]).flag;
    _462::real_t split_value = GetSplitValueByIndex(p, median, flag);
    _462::real_t p_value = GetSplitValueByPhoton(point, flag);
    if (p_value <= split_value) {
      LookuptKDTree(p, point, norm, heap, begin, median, distance);
      AddNeighbor(p[median].pos, p[median].dir, point, norm, heap, median,
                  distance, kMaxDistance, kNumberOfPhotonsNeayby);
      if (heap.size() < kNumberOfPhotonsNeayby ||
          (p_value - split_value) * (p_value - split_value) < distance)
        LookuptKDTree(p, point, norm, heap, median + 1, end, distance);
    } else {
      LookuptKDTree(p, point, norm, heap, median + 1, end, distance);
      AddNeighbor(p[median].pos, p[median].dir, point, norm, heap, median,
                  distance, kMaxDistance, kNumberOfPhotonsNeayby);
      if (heap.size() < kNumberOfPhotonsNeayby ||
          (p_value - split_value) * (p_value - split_value) < distance)
        LookuptKDTree(p, point, norm, heap, begin, median, distance);
    }
  }
}

_462::Vector3 PhotonSimulator::GetPixelColor(const _462::Vector3 &ray_pos,
                                             const _462::Vector3 &ray_dir) {
  _462::Vector3 direct, global;
  Face *min_face = nullptr;
  Mesh *min_mesh = nullptr;
  Model *min_model = nullptr;
  Texture texture_info;
  std::tie(min_model, min_mesh, min_face) =
      FindFirstIntersect(ray_pos, ray_dir);
  _462::Vector3 result;
  if (min_face != nullptr) {
    _462::Vector3 intersect =
        min_model->GetIntersect(*min_face, ray_pos, ray_dir);
    direct = min_model->GetFaceTextureColor(*min_face, *min_mesh, intersect);

    global = _462::Vector3(0.0, 0.0, 0.0);
    _462::real_t d = 0.025;
    int count = 0;
    std::vector<Neighbor> neighbors;
    make_heap(neighbors.begin(), neighbors.end());
    LookuptKDTree(absorb_photons, intersect, min_face->normal, neighbors, 0,
                  absorb_photons.size() - 1, d);
    for (auto &neighbor:neighbors) {
      _462::real_t dist =
          _462::distance(absorb_photons[neighbor.i].pos, intersect);
      _462::Vector3 color = absorb_photons[neighbor.i].power;
      if (dist >= 1.0f) {
        color /= dist;
      }
      _462::real_t dr1 =
          _462::dot(absorb_photons[neighbor.i].dir,
                                   absorb_photons[neighbor.i].pos - intersect);
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

RadianceResult PhotonSimulator::RussianRoulette(const _462::real_t abr,
                                                const _462::real_t ref,
                                                const _462::real_t trans) {
  _462::real_t a = (rand() % 100) / 100.0f;
  if (a < abr)
    return kAbsorb;
  else if (a < abr + ref)
    return kReflect;
  else
    return kRefract;
}

std::tuple<Model *, Mesh *, Face *> PhotonSimulator::FindFirstIntersect(
    const _462::Vector3 &pos, const _462::Vector3 &dir) {
  Face *min_face = nullptr;
  Mesh *min_mesh = nullptr;
  Model *min_model = nullptr;
  _462::real_t min_distance = std::numeric_limits<double>::max();
  for (auto &model : models) {
    Face *face = nullptr;
    Mesh *mesh = nullptr;
    _462::real_t distance = model->FindFirstIntersect(&face, &mesh, pos, dir);
    if (distance < min_distance) {
      min_face = face;
      min_mesh = mesh;
      min_model = model;
    }
  }
  return std::make_tuple(min_model, min_mesh, min_face);
}

void PhotonSimulator::PhotonsModify() {
  while (!alive_photons.empty()) {
    for (int i = 0; i < alive_photons.size(); i++) {
      Face *min_face = nullptr;
      Mesh *min_mesh = nullptr;
      Model *min_model = nullptr;
      std::tie(min_model, min_mesh, min_face) =
          FindFirstIntersect(alive_photons[i].pos, alive_photons[i].dir);
      if (min_face != nullptr) {
        _462::Vector3 intersect = min_model->GetIntersect(
            *min_face, alive_photons[i].pos, alive_photons[i].dir);
        RadianceResult res = RussianRoulette(min_face->material.aborption,
                                             min_face->material.reflection,
                                             min_face->material.transmision);
        switch (res) {
          case kAbsorb: {
            absorb_photons.push_back(
                Photon(min_face->normal, intersect, alive_photons[i].power));
            alive_photons.erase(alive_photons.begin() + i--);
            min_face->photons++;
            break;
          }
          case kReflect: {
            _462::Vector3 ref =
                GetReflect(alive_photons[i].dir, min_face->normal);
            alive_photons[i].pos = intersect;
            alive_photons[i].dir = ref;
            break;
          }
          case kRefract: {
            _462::Vector3 ref =
                GetRefract(alive_photons[i].dir, min_face->normal, 1.0);
            alive_photons[i].pos = intersect;
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
  if (isRendering) {
    ConstructKDTree(absorb_photons, 0, absorb_photons.size());
  }
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
