#ifndef COMPUTATIONAL_AGROECOLOGY_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_SIMULATOR_H_

#include <chrono>

// forward declaration
namespace environment {
class Environment;
}

namespace simulator {

class Simulator {
 public:
  virtual void SimulateToTime(
      environment::Environment* env,
      const std::chrono::system_clock::time_point& time) = 0;
};

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_SIMULATOR_H_