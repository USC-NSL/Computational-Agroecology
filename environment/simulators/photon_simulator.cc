#include "photon_simulator.h"

#include "environment.h"

namespace simulator {

namespace photonsimulator {

// TODO: @Hangjie change number to meaningful name
PhotonSimulator::PhotonSimulator(const int number, const real_t distance,
                                 const real_t height)
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
  photon_emit(Vector3(-sin(env->sun_info().SunAzimuth),
                      -cos(env->sun_info().SunAzimuth),
                      -cos(env->sun_info().SolarAltitude)),
              Vector3(env->sun_info().HourlyIrradiance,
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
  photons_modify();

  // write result to env
  WriteResult2Env(env);
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
void PhotonSimulator::WriteResult2Env(environment::Environment *env){}

void PhotonSimulator::photon_emit(
    const Vector3 &sun_direction, const Vector3 &sun_strength,
    const double latitude_bottom, const double latitude_top,
    const double latitudeDiff, const double longitude_left,
    const double longitude_right, const double longitudeDiff) {
  for (real_t i = (real_t)latitude_bottom; i <= (real_t)latitude_top;
       i += (real_t)latitudeDiff) {
    for (real_t j = (real_t)longitude_left; j <= (real_t)longitude_right;
         j += (real_t)longitudeDiff) {
      alive_photons.push_back(
          Photon(sun_direction, Vector3(i, j, kSunHeight), sun_strength));
    }
  }
}

void PhotonSimulator::construct_kdtree(std::vector<Photon> &p,
                                       const unsigned int begin,
                                       const unsigned int end) {
  if (end - begin == 0)
    return;
  if (end - begin == 1) {
    p[begin].flag = kLEAF;
    return;
  }
  unsigned median = begin + (end - begin) / 2;
  real_t x_avg = 0.0, y_avg = 0.0, z_avg = 0.0;
  real_t x_var = 0.0, y_var = 0.0, z_var = 0.0;
  real_t n = (real_t)(end - begin);
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
  real_t max_var = std::max(std::max(x_var, y_var), z_var);
  if (max_var == x_var) {
    std::sort(p.begin() + begin, p.begin() + end, compare_x);
    p[median].flag = kXAXIS;
  }
  if (max_var == y_var) {
    std::sort(p.begin() + begin, p.begin() + end, compare_y);
    p[median].flag = kYAXIS;
  }
  if (max_var == z_var) {
    std::sort(p.begin() + begin, p.begin() + end, compare_z);
    p[median].flag = kZAXIS;
  }
  construct_kdtree(p, begin, median);
  construct_kdtree(p, median + 1, end);
}

void PhotonSimulator::lookup_kdtree(const std::vector<Photon> &p,
                                    const Vector3 &point, const Vector3 &norm,
                                    Neighbor *neighbors,
                                    const unsigned int begin,
                                    const unsigned int end, real_t &distance,
                                    int &size) {
  if (begin == end)
    return;
  else if (begin + 1 == end)
    add_neighbor(p[begin].pos, p[begin].dir, point, norm, neighbors, begin,
                 distance, kMaxDistance, size, kNumberOfPhotonsNeayby);
  else {
    unsigned int median = begin + (end - begin) / 2;
    int flag = (p[median]).flag;
    real_t split_value = get_split(p, median, flag);
    real_t p_value = get_p(point, flag);
    if (p_value <= split_value) {
      lookup_kdtree(p, point, norm, neighbors, begin, median, distance, size);
      add_neighbor(p[median].pos, p[median].dir, point, norm, neighbors, median,
                   distance, kMaxDistance, size, kNumberOfPhotonsNeayby);
      if (size < kNumberOfPhotonsNeayby ||
          (p_value - split_value) * (p_value - split_value) < distance)
        lookup_kdtree(p, point, norm, neighbors, median + 1, end, distance,
                      size);
    } else {
      lookup_kdtree(p, point, norm, neighbors, median + 1, end, distance, size);
      add_neighbor(p[median].pos, p[median].dir, point, norm, neighbors, median,
                   distance, kMaxDistance, size, kNumberOfPhotonsNeayby);
      if (size < kNumberOfPhotonsNeayby ||
          (p_value - split_value) * (p_value - split_value) < distance)
        lookup_kdtree(p, point, norm, neighbors, begin, median, distance, size);
    }
  }
}

Vector3 PhotonSimulator::get_Intersect(const Vector3 &p1, const Vector3 &p2,
                                       const Vector3 &p3,
                                       const Vector3 &line_point,
                                       const Vector3 &line_dir) {
  Vector3 plane_normal = get_Normal(p1, p2, p3);
  real_t d = dotresult(p1 - line_point, plane_normal) /
             dotresult(line_dir, plane_normal);
  normalize(line_dir);
  return d * line_dir + line_point;
}

bool PhotonSimulator::in_Triangle(const Vector3 &a, const Vector3 &b,
                                  const Vector3 &c, const Vector3 &p) {
  Vector3 v0 = c - a;
  Vector3 v1 = b - a;
  Vector3 v2 = p - a;
  real_t dot00 = dotresult(v0, v0);
  real_t dot01 = dotresult(v0, v1);
  real_t dot02 = dotresult(v0, v2);
  real_t dot11 = dotresult(v1, v1);
  real_t dot12 = dotresult(v1, v2);
  real_t inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
  real_t u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
  if (u < 0 || u > 1)
    return false;
  real_t v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
  if (v < 0 || v > 1)
    return false;
  return u + v <= 1;
}

Vector3 PhotonSimulator::get_Normal(const Vector3 &p1, const Vector3 &p2,
                                    const Vector3 &p3) {
  real_t a = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
  real_t b = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
  real_t c = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
  return Vector3(a, b, c);
}

Vector3 PhotonSimulator::get_pixel_color(const Vector3 &ray_pos,
                                         const Vector3 &ray_dir) {
  Vector3 direct, global;
  Vector3 p(1000.0f, 1000.0f, 0.0f);
  Face *min = NULL;
  GLuint texture_id;
  Model *min_model;
  Vector3 min_normal;
  for (auto &model : models) {
    for (auto &mesh : model->meshes) {
      for (auto &face : mesh.faces) {
        Vector3 a = model->vertices[face.vertex1.vi] + model->rel_pos;
        Vector3 b = model->vertices[face.vertex2.vi] + model->rel_pos;
        Vector3 c = model->vertices[face.vertex3.vi] + model->rel_pos;
        Vector3 normal = get_Normal(a, b, c);
        Vector3 intersect = get_Intersect(a, b, c, ray_pos, ray_dir);
        if (in_Triangle(a, b, c, intersect)) {
          if (distance(ray_pos, intersect) < distance(ray_pos, p)) {
            p = intersect;
            min = &face;
            texture_id = mesh.texture_id;
            min_model = model;
            if (dotresult(normal, ray_dir) > 0.0f) {
              normal *= -1.0;
            }
            min_normal = normal;
          }
        }
      }
    }
  }
  Vector3 result;
  if (min) {
    Vector2 texcoord = min->getTexcoord(
        p - min_model->rel_pos, min_model->vertices, min_model->texcoords);
    Texture texture_info = min_model->getTextureInfo(texture_id);
    int x =
        ((int)(texture_info.w * texcoord.x) % texture_info.w + texture_info.w) %
        texture_info.w;
    int y =
        ((int)(texture_info.h * texcoord.y) % texture_info.h + texture_info.h) %
        texture_info.h;
    direct = Vector3(
        texture_info.texture[3 * (x + y * texture_info.w)] / 255.0f,
        texture_info.texture[3 * (x + y * texture_info.w) + 1] / 255.0f,
        texture_info.texture[3 * (x + y * texture_info.w) + 2] / 255.0f);

    global = Vector3(0.0, 0.0, 0.0);
    int size = 0;
    float d = 0.0;
    int count = 0;
    Neighbor neighbors[kNumberOfPhotonsNeayby];
    lookup_kdtree(absorb_photons, p, min_normal, neighbors, 0,
                  absorb_photons.size() - 1, d, size);
    for (int i = 0; i < size; i++) {
      real_t dist = distance(absorb_photons[neighbors[i].i].pos, p);
      Vector3 color = absorb_photons[neighbors[i].i].power;
      if (dist >= 1.0f) {
        color /= dist;
      }
      real_t dr1 = dotresult(absorb_photons[neighbors[i].i].dir,
                             absorb_photons[neighbors[i].i].pos - p);
      global += color;
      count++;
    }
    if (count) {
      global /= (real_t)count;
      result = dot(direct, global);
    } else {
      result = Vector3(0.0f, 0.0f, 0.0f);
    }
  } else {
    result = Vector3(0.0f, 0.0f, 0.0f);
  }
  return result;
}

Vector3 PhotonSimulator::get_ray_color(const int x, const int y,
                                       const int scene_length,
                                       const int scene_width,
                                       const Vector3 &camera_pos,
                                       const Vector3 &camera_ctr,
                                       const Vector3 &camera_up) {
  Vector3 dir = get_ray_dir(x, y, scene_length, scene_width, camera_pos,
                            camera_ctr, camera_up);
  Vector3 color = get_pixel_color(camera_pos, dir);
  return color;
}

Vector3 PhotonSimulator::get_ray_dir(const int x, const int y,
                                     const int scene_length,
                                     const int scene_width,
                                     const Vector3 &camera_pos,
                                     const Vector3 &camera_ctr,
                                     const Vector3 &camera_up) {
  Vector3 dir = camera_ctr - camera_pos;
  Vector3 cR = cross(dir, camera_up);
  Vector3 cU = cross(cR, dir);
  real_t AR = length(cU) / length(cR);
  real_t dist = tan(kPI / 4) * 2 / length(dir);
  Vector3 t =
      dir +
      dist * (((real_t)y - (real_t)scene_width / 2) / scene_width * cU +
              AR * ((real_t)x - (real_t)scene_length / 2) / scene_length * cR);
  t = normalize(t);
  return t;
}

int PhotonSimulator::Russian_roulette(const real_t abr, const real_t ref,
                                      const real_t trans) {
  real_t a = (rand() % 100) / 100.0f;
  if (a < abr)
    return kAbsorb;
  else if (a < abr + ref)
    return kReflect;
  else
    return kTrans;
}

void PhotonSimulator::photons_modify() {
  while (!alive_photons.empty()) {
    for (int i = 0; i < alive_photons.size(); i++) {
      Vector3 p(-100.0f, -100.0f, -100.0f);
      Face *min = NULL;
      Model *min_model;
      Vector3 min_normal;
      for (auto &model : models) {
        for (auto &mesh : model->meshes) {
          for (auto &face : mesh.faces) {
            Vector3 a = model->vertices[face.vertex1.vi] + model->rel_pos;
            Vector3 b = model->vertices[face.vertex2.vi] + model->rel_pos;
            Vector3 c = model->vertices[face.vertex3.vi] + model->rel_pos;
            Vector3 normal = get_Normal(a, b, c);
            Vector3 intersect = get_Intersect(a, b, c, alive_photons[i].pos,
                                              alive_photons[i].dir);
            if (in_Triangle(a, b, c, intersect)) {
              if (distance(alive_photons[i].pos, intersect) <
                  distance(alive_photons[i].pos, p)) {
                p = intersect;
                min = &face;
                min_model = model;
                if (dotresult(normal, alive_photons[i].dir) > 0.0f) {
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
            Russian_roulette(min->material.aborption, min->material.reflection,
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
            Vector3 ref = get_reflect(alive_photons[i].dir, min_normal);
            alive_photons[i].pos = p;
            alive_photons[i].dir = ref;
            break;
          }
          case kTrans: {
            Vector3 ref = get_refract(alive_photons[i].dir, min_normal, 1.0);
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
  construct_kdtree(absorb_photons, 0, absorb_photons.size());
}

Vector3 PhotonSimulator::get_reflect(const Vector3 &dir, const Vector3 &norm) {
  Vector3 normal = normalize(norm);
  return (dir - 2 * dot((dot(dir, normal)), normal));
}

Vector3 PhotonSimulator::get_refract(const Vector3 &dir, const Vector3 &norm,
                                     real_t coef) {
  return Vector3(-sqrt(1 - coef * coef *
                               (1 - squared_length(dot(dir, norm)) *
                                        squared_length(dot(dir, norm)))) *
                     norm +
                 coef * (dir + squared_length(dot(dir, norm)) * norm));
}

}  // namespace photonsimulator

}  // namespace simulator
