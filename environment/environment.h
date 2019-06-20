#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENVIRONMENT_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENVIRONMENT_H_

#include <chrono>
#include <iostream>
#include <vector>

#include "climate.h"
#include "config.h"
#include "simulators/main_simulator.h"
#include "simulators/sun_simulator.h"
#include "terrain.h"
#include "weather.h"

namespace environment {

// The main data structure which stores most of the data about this environment
// TODO: Copy-on-Write
class Environment {
 public:
  Environment(const Config& config,
              const std::chrono::system_clock::time_point& time,
              const Terrain& terrain);

  // This tells its internal simulator to simulate to the specified time
  void JumpToTime(const std::chrono::system_clock::time_point& time);

  // This tells its internal simulator to move forward some time period
  void JumpDuration(const std::chrono::duration<int>& duration);

  // This receives an action from either an agent or a human
  // telling the internal simulator to schedule a task
  void ReceiveAction(const simulator::action::Action* action);

  // This is identical to the member function above except that a list of
  // actions are received here
  void ReceiveActions(const simulator::action::ActionList& actions);

  // Accessors
  inline const Config& config() { return config_; }

  inline const Climate& climate() { return climate_; }

  inline const std::chrono::system_clock::time_point& timestamp() {
    return timestamp_;
  }

  inline const Terrain& terrain() { return terrain_; }

  inline const Weather& weather() { return weather_; }

 private:
  friend std::ostream& operator<<(std::ostream& os, const Environment& env);
  friend struct simulator::action::ActionExecutorList;
  friend class simulator::MainSimulator;
  friend class simulator::SunSimulator;

  Config config_;
  const Climate climate_;

  //Current (longitude, latitude) pair and variables just for the sun simulator
  float longitude_, latitude_;
  float sunAzimuth, solarAltitude, hourlyIrradiance;

  // Current time in this environment
  std::chrono::system_clock::time_point timestamp_;
  Terrain terrain_;
  Weather weather_;
  // TODO: define a class for light information

  simulator::MainSimulator main_simulator_;
};

std::ostream& operator<<(std::ostream& os, const Environment& env);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENVIRONMENT_H_