#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_

#include <tuple>
#include <vector>

#include "photons/model/model.h"
#include "photons/photon/neighbor.h"
#include "photons/photon/photon.h"
#include "photons/photon_simulator_config.h"
#include "simulator.h"

namespace simulator {

namespace photonsimulator {

class PhotonSimulator : public Simulator {
 public:
  PhotonSimulator(const int number, const _462::real_t distance,
                  const _462::real_t height);
  void SimulateToTime(
      environment::Environment *env,
      const std::chrono::system_clock::time_point &time) override;

  // Ralph: private member variable names should have trailing undderscores
  // Ralph: I think it would be better if we separate variables and functions
  // Ralph: for read-only functions, put const at the end of function
  // declarations
 private:
  // common
  enum class RadianceResult { kAbsorb = 0, kReflect, kRefract };

  // TODO: add interface if necessary
  bool isRendering_ = false;

  // the part for model
  // Ralph: I have changed it back to `vector`.
  std::vector<Model> models_;

  // the part for photon
  const int kNumberOfPhotonsNearby_;
  const _462::real_t kMaxDistance_;
  const _462::real_t kSunHeight_;
  std::vector<Photon> alive_photons_, absorb_photons_;

  // emit all photons to the space by specific parameters
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

  // this function is used to randomize the result of one photon, the parameters
  // are three posibilities
  RadianceResult RussianRoulette(const _462::real_t abr, const _462::real_t ref,
                                 const _462::real_t tran) const;

  void ConstructKDTree(std::vector<Photon> &p, const unsigned int begin,
                       const unsigned int end);

  // heap is the min-heap returned by the function, distance is the max_distance
  // for the nearest photon for the special point
  void LookuptKDTree(const std::vector<Photon> &p, const _462::Vector3 &point,
                     const _462::Vector3 &norm, std::vector<Neighbor> *heap,
                     const unsigned int begin, const unsigned int end,
                     _462::real_t *distance);

  // return the pixel RGB value
  _462::Vector3 GetPixelColor(const _462::Vector3 &ray_pos,
                              const _462::Vector3 &ray_dir);

  // return the direction of the certain ray by coordinate x and y
  _462::Vector3 GetRayDir(const int x, const int y, const int scene_length,
                          const int scene_width,
                          const _462::Vector3 &camera_pos,
                          const _462::Vector3 &camera_ctr,
                          const _462::Vector3 &camera_up) const;

  // return the pixel color for the ray identified by coord x and y, call
  // GetPixelColor inside
  _462::Vector3 GetRayColor(const int x, const int y, const int scene_length,
                            const int scene_width,
                            const _462::Vector3 &camera_pos,
                            const _462::Vector3 &camera_ctr,
                            const _462::Vector3 &camera_up);

  // get the reflect direction of the input dir and the normal
  _462::Vector3 GetReflect(const _462::Vector3 &dir,
                           const _462::Vector3 &norm) const;

  // get the refract direction of the input dir and the normal
  _462::Vector3 GetRefract(const _462::Vector3 &dir, const _462::Vector3 &norm,
                           _462::real_t coef) const;

  // this function will return the Model, Mesh, and Face that is first hitted by
  // certain ray identified by pos and dir
  std::tuple<Model *, Mesh *, Face *> FindFirstIntersect(
      const _462::Vector3 &pos, const _462::Vector3 &dir);

  // write result to environment
  void WriteResultToEnv(environment::Environment *env);

  // free 3d-obj model from memory
  void FreeModels();

  // load 3d-obj model from disk
  void LoadModels(environment::Environment *env);
};

}  // namespace photonsimulator

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
