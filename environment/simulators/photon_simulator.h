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
  PhotonSimulator(const int number, const _462::real_t distance,
                  const _462::real_t height);
  void SimulateToTime(
      environment::Environment *env,
      const std::chrono::system_clock::time_point &time) override;

 private:
  // common
  // TODO: add interface if necessary
  bool isRendering = false;

  // the part for model
  std::vector<Model *> models;
  void FreeModels();
  void LoadModels(environment::Environment *env);

  // the part for photon
  const int kNumberOfPhotonsNeayby;
  const _462::real_t kMaxDistance;
  const _462::real_t kSunHeight;
  std::vector<Photon> alive_photons, absorb_photons;
  void PhotonEmit(const _462::Vector3 &sun_direction,
                  const _462::Vector3 &sun_strength,
                  const double latitude_bottom, const double latitude_top,
                  const double latitudeDiff, const double longitude_left,
                  const double longitude_right, const double longitudeDiff);
  void PhotonsModify();
  int RussianRoulette(const _462::real_t abr, const _462::real_t ref,
                      const _462::real_t tran);
  void ConstructKDTree(std::vector<Photon> &p, const unsigned int begin,
                       const unsigned int end);
  void LookuptKDTree(const std::vector<Photon> &p, const _462::Vector3 &point,
                     const _462::Vector3 &norm, Neighbor *neighbors,
                     const unsigned int begin, const unsigned int end,
                     _462::real_t &distance, int &size);
  _462::Vector3 GetIntersect(const Face &face,
                             const std::vector<_462::Vector3> &vertices,
                             const _462::Vector3 &line_point,
                             const _462::Vector3 &line_dir);
  bool IsInTriangle(const _462::Vector3 &a, const _462::Vector3 &b,
                    const _462::Vector3 &c, const _462::Vector3 &p);
  _462::Vector3 GetPixelColor(const _462::Vector3 &ray_pos,
                              const _462::Vector3 &ray_dir);
  _462::Vector3 GetRayDir(const int x, const int y, const int scene_length,
                          const int scene_width,
                          const _462::Vector3 &camera_pos,
                          const _462::Vector3 &camera_ctr,
                          const _462::Vector3 &camera_up);
  _462::Vector3 GetRayColor(const int x, const int y, const int scene_length,
                            const int scene_width,
                            const _462::Vector3 &camera_pos,
                            const _462::Vector3 &camera_ctr,
                            const _462::Vector3 &camera_up);
  _462::Vector3 GetReflect(const _462::Vector3 &dir, const _462::Vector3 &norm);
  _462::Vector3 GetRefract(const _462::Vector3 &dir, const _462::Vector3 &norm,
                           _462::real_t coef);

  // write result to environment
  void WriteResultToEnv(environment::Environment *env);
};

}  // namespace photonsimulator

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
