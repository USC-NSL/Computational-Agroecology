#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_

#include "photon_simulator/photon_simulator_config.h"
#include "simulator.h"

#include <vector>

namespace simulator {

namespace photonsimulator {

const int kAborb = 0;
const int kReflect = 1;
const int kTrans = 2;

class PhotonSimulator : public Simulator {
 public:
  PhotonSimulator(const int number, const real_t distance, const real_t height);
  void SimulateToTime(
      environment::Environment* env,
      const std::chrono::system_clock::time_point& time) override;

 private:
  // the part for photon
  const int kNumberOfPhotonsNeayby;
  const real_t kMaxDistance;
  const real_t kSunHeight;
  std::vector<Photon> alive_photons, absorb_photons;
  std::vector<Model*> models;
  void photon_emit(const Vector3& sun_direction, const Vector3& sun_strength,
                   const double latitude_bottom, const double latitude_top,
                   const double latitudeDiff, const double longitude_left,
                   const double longitude_right, const double longitudeDiff);
  void photons_modify();
  int Russian_roulette(const real_t abr, const real_t ref, const real_t tran);
  void construct_kdtree(std::vector<Photon>& p, const unsigned int begin,
                        const unsigned int end);
  void lookup_kdtree(std::vector<Photon>& p, const Vector3& point,
                     const Vector3& norm, Neighbor* neighbors,
                     const unsigned int begin, const unsigned int end,
                     real_t& distance, int& size);
  Vector3 get_Intersect(const Vector3& p1, const Vector3& p2, const Vector3& p3,
                        const Vector3& line_point, const Vector3& line_dir);
  bool in_Triangle(const Vector3& a, const Vector3& b, const Vector3& c,
                   const Vector3& p);
  Vector3 get_Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3);
  Vector3 get_pixel_color(const Vector3& ray_pos, const Vector3& ray_dir);
  Vector3 get_ray_dir(const int x, const int y, const int scene_length,
                      const int scene_width, const Vector3& camera_pos,
                      const Vector3& camera_ctr, const Vector3& camera_up);
  Vector3 get_ray_color(const int x, const int y, const int scene_length,
                        const int scene_width, const Vector3& camera_pos,
                        const Vector3& camera_ctr, const Vector3& camera_up);
  Vector3 get_reflect(const Vector3& dir, const Vector3& norm);
  Vector3 get_refract(const Vector3& dir, const Vector3& norm, real_t coef);
  Vector3 get_Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3);
};

}  // namespace photonsimulator

}  // namespace simulator
#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
