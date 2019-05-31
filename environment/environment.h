#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_H_

#include <chrono>
#include <iostream>
#include <vector>

#include "climate.h"
#include "config.h"
#include "simulators/main_simulator.h"
#include "terrain.h"
#include "weather.h"

namespace environment {

class Environment {
 public:
  Environment(const Config& config,
              const std::chrono::system_clock::time_point& time,
              const Terrain& terrain);

  void JumpToTime(const std::chrono::system_clock::time_point& time);
  void JumpDuration(const std::chrono::duration<int>& duration);

  void ReceiveAction(const simulator::action::Action* action);
  void ReceiveActions(
      const std::vector<const simulator::action::Action*>& actions);

  inline const Config& config() { return config_; }

  inline const Climate& climate() { return climate_; }

  inline const std::chrono::system_clock::time_point& timestamp() {
    return timestamp_;
  }

  inline const Terrain& terrain() { return terrain_; }

  inline const Weather& weather() { return weather_; }

 private:
  Config config_;
  const Climate climate_;

  std::chrono::system_clock::time_point timestamp_;
  Terrain terrain_;
  Weather weather_;

  // TODO: define a class for light information

  simulator::MainSimulator main_simulator_;

  friend std::ostream& operator<<(std::ostream& os, const Environment& env);

  friend struct simulator::action::ActionList;

  friend class simulator::MainSimulator;
};

std::ostream& operator<<(std::ostream& os, const Environment& env);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_H_