#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_

#include <vector>
#include <tuple>

#include "photons/model/model.h"
#include "photons/photon/neighbor.h"
#include "photons/photon/photon.h"
#include "photons/photon_simulator_config.h"
#include "simulator.h"

namespace simulator {

namespace photonsimulator {

typedef enum {kAbsorb = 0, kReflect = 1, kRefract = 2} RadianceResult;

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

  // free 3d-obj model from memory
  void FreeModels();
  // load 3d-obj model from disk
  void LoadModels(environment::Environment *env);

  // the part for photon
  const int kNumberOfPhotonsNeayby;
  const _462::real_t kMaxDistance;
  const _462::real_t kSunHeight;
  std::vector<Photon> alive_photons, absorb_photons;

  //emit all photons to the space by specific parameters
  /*** 
  @para:
  latitudeDiff : the step for latitude increment;
  longitudeDiff : the step for longitude increment;
  ***/
  void PhotonEmit(const _462::Vector3 &sun_direction,
                  const _462::Vector3 &sun_strength,
                  const double latitude_bottom, const double latitude_top,
                  const double latitudeDiff, const double longitude_left,
                  const double longitude_right, const double longitudeDiff);

  // let all photons transmit in the space
  void PhotonsModify();
  RadianceResult RussianRoulette(const _462::real_t abr, const _462::real_t ref,
                      const _462::real_t tran);
  void ConstructKDTree(std::vector<Photon> &p, const unsigned int begin,
                       const unsigned int end);
  // heap is the min-heap returned by the function, distance is the max_distance for the nearest photon for the special point
  void LookuptKDTree(const std::vector<Photon> &p, const _462::Vector3 &point,
                     const _462::Vector3 &norm, std::vector<Neighbor> &heap,
                     const unsigned int begin, const unsigned int end,
                     _462::real_t &distance);
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
  std::tuple<Model *, Mesh *, Face *> FindFirstIntersect(
      const _462::Vector3 &pos, const _462::Vector3 &dir);
  // write result to environment
  void WriteResultToEnv(environment::Environment *env);
};

}  // namespace photonsimulator

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_

