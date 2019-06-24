#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_SIMULATOR_H_

#include <chrono>

// forward declaration to avoid compilation errors
namespace environment {
class Environment;
}

namespace simulator {

// This is the abstract definition for simulators.
class Simulator {
 public:
  // Given a time point, the simulator should change the environment.
  virtual void SimulateToTime(
      environment::Environment* env,
      const std::chrono::system_clock::time_point& time) = 0;
};

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_SIMULATOR_H_