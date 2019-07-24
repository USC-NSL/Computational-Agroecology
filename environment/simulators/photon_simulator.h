#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_

#include <vector>

#include "photons/model/model.h"
#include "photons/photon/neighbor.h"
#include "photons/photon/photon.h"
#include "photons/photon_simulator_config.h"
#include "simulator.h"

namespace simulator {

namespace photonsimulator {

const int kAbsorb = 0;
const int kReflect = 1;
const int kTrans = 2;

class PhotonSimulator : public Simulator {
 public:
  PhotonSimulator(const int number, const real_t distance, const real_t height);
  void SimulateToTime(
      environment::Environment *env,
      const std::chrono::system_clock::time_point &time) override;

 private:
  // the part for model
  std::vector<Model *> models;
  void FreeModels();
  void LoadModels(environment::Environment *env);

  // the part for photon
  const int kNumberOfPhotonsNeayby;
  const real_t kMaxDistance;
  const real_t kSunHeight;
  std::vector<Photon> alive_photons, absorb_photons;
  void PhotonEmit(const Vector3 &sun_direction, const Vector3 &sun_strength,
                   const double latitude_bottom, const double latitude_top,
                   const double latitudeDiff, const double longitude_left,
                   const double longitude_right, const double longitudeDiff);
  void PhotonsModify();
  int RussianRoulette(const real_t abr, const real_t ref, const real_t tran);
  void ConstructKDTree(std::vector<Photon> &p, const unsigned int begin,
                        const unsigned int end);
  void LookuptKDTree(const std::vector<Photon> &p, const Vector3 &point,
                     const Vector3 &norm, Neighbor *neighbors,
                     const unsigned int begin, const unsigned int end,
                     real_t &distance, int &size);
  Vector3 GetIntersect(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
                        const Vector3 &line_point, const Vector3 &line_dir);
  bool IsInTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c,
                   const Vector3 &p);
  Vector3 GetNormal(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);
  Vector3 GetPixelColor(const Vector3 &ray_pos, const Vector3 &ray_dir);
  Vector3 GetRayDir(const int x, const int y, const int scene_length,
                      const int scene_width, const Vector3 &camera_pos,
                      const Vector3 &camera_ctr, const Vector3 &camera_up);
  Vector3 GetRayColor(const int x, const int y, const int scene_length,
                        const int scene_width, const Vector3 &camera_pos,
                        const Vector3 &camera_ctr, const Vector3 &camera_up);
  Vector3 GetReflect(const Vector3 &dir, const Vector3 &norm);
  Vector3 GetRefract(const Vector3 &dir, const Vector3 &norm, real_t coef);

  // write result to environment
  void WriteResultToEnv(environment::Environment *env);
};

}  // namespace photonsimulator

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
