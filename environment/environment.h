#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENVIRONMENT_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENVIRONMENT_H_

#include <chrono>
#include <iostream>
#include <queue>
#include <vector>

#include "environment/climate.h"
#include "environment/config.h"
#include "environment/suninfo.h"
#include "environment/terrain.h"
#include "environment/weather.h"

namespace environment {

// The main data structure which stores most of the data about this environment
// TODO: Copy-on-Write
class Environment {
 public:
  Environment(const Config &config,
              const std::chrono::system_clock::time_point &time,
              const std::chrono::duration<int> &time_step_length,
              const Terrain &terrain);

  // Jump to a specified time_step in the timeline.
  void JumpToTimeStep(const int64_t time_step);

  // Jump forward a number of time_steps in the timeline.
  void JumpForwardTimeStep(const int64_t time_step_num);

  // This receives an action from either an agent or a human
  // telling the internal simulator to schedule a task
  void ReceiveAction(const agent::action::Action *action);

  // This is identical to the member function above except that a list of
  // actions are received here
  void ReceiveActions(const agent::action::ActionList &actions);

  // TODO: define it
  const int score() const;

  // Accessors
  inline const Config &config() const { return config_; }
  inline const Climate &climate() const { return climate_; }
  inline const std::chrono::system_clock::time_point &timestamp() const {
    return timestamp_;
  }
  inline const std::chrono::duration<int> &time_step_length() const {
    return time_step_length_;
  }
  inline const int64_t &time_step() const { return time_step_; }
  inline const SunInfo &sun_info() const { return sun_info_; }
  inline const Terrain &terrain() const { return terrain_; }
  inline const Weather &weather() const { return weather_; }

 private:
  friend std::ostream &operator<<(std::ostream &os, const Environment &env);

  Config config_;
  const Climate climate_;

  // the information of sun from the simulator
  SunInfo sun_info_;

  // Current time in this environment
  std::chrono::system_clock::time_point timestamp_;
  // The length of a time step
  const std::chrono::duration<int> time_step_length_;
  // Indicates the time step in which this environment is in
  int64_t time_step_;

  Terrain terrain_;
  Weather weather_;
  // TODO: define a class for light information

  // Simulators:

  // Given a future time step, push actions which is starting before the time
  // step into the `starting_action_pq_` from `action_pq_` and make actions in
  // `starting_action_pq_` which has completed before the time step take effect.
  // These two are done in chronological order.
  // In `action`, we have its start time and end time. As the time step goes
  // beyond its start time, it should be pushed into `starting_action_pq_`. As
  // the time step goes beyond its end time, it should be poped from the
  // `starting_action_pq_` and take effect.
  void SyncActionPqToTimeStep(const int64_t time_step);

  // Simulate this environment to a time point
  void SimulateToTimeStep(const int64_t time_step);

  // This PQ collects all actions sent from an agent
  std::priority_queue<const agent::action::Action *,
                      std::vector<const agent::action::Action *>,
                      agent::action::ActionStartTimeComparator>
      action_pq_;

  // This PQ collects all actions that are starting but have not taken effect.
  std::priority_queue<const agent::action::Action *,
                      std::vector<const agent::action::Action *>,
                      agent::action::ActionEndTimeComparator>
      starting_action_pq_;
};

std::ostream &operator<<(std::ostream &os, const Environment &env);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_ENVIRONMENT_H_