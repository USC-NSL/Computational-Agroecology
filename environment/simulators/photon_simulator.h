#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_

#include "simulator.h"

namespace simulator {

class PhotonSimulator : public Simulator {
 public:
  PhotonSimulator();
  void SimulateToTime(
      environment::Environment* env,
      const std::chrono::system_clock::time_point& time) override;

 private:
  
};

}  // namespace simulator
#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_SIMULATOR_H_
