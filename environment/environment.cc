#include "environment.h"

#include <ctime>

namespace environment {

Environment::Environment(const config::Config &config,
                         const config::TerrainRawData &terrain_raw_data,
                         const std::chrono::system_clock::time_point &time,
                         const std::chrono::duration<int> &time_step_length)
    : config_(config),
      climate_(config),
      timestamp_(time),
      time_step_length_(time_step_length),
      time_step_(0),
      weather_(0.0, 0.0, 0.0, 0.0, 0.0,
               0.0),  // TODO: Get weather data and put them into this struct.
      meteorology_(time, config.location, climate_.climate_zone, weather_),
      terrain_(terrain_raw_data, meteorology_) {  // TODO: Create some data structure here
  auto to_round = timestamp_.time_since_epoch() % time_step_length_;
  timestamp_ -= to_round;
}

void Environment::JumpToTimeStep(const int64_t time_step) {
  SyncActionPqToTimeStep(time_step);
  SimulateToTimeStep(time_step);
}

void Environment::JumpForwardTimeStep(const int64_t time_step_num) {
  JumpToTimeStep(time_step_ + time_step_num);
}

void Environment::ReceiveAction(const agent::action::Action *action) {
  ReceiveActions(agent::action::ActionList(1, action));
}

void Environment::ReceiveActions(const agent::action::ActionList &actions) {
  for (const auto &action : actions) {
    action_pq_.push(action);
  }
}

void Environment::SyncActionPqToTimeStep(const int64_t time_step) {
  // Handle one action per loop.
  while (!action_pq_.empty() || !starting_action_pq_.empty()) {
    // `action_pq_` is empty or the start time of the very first action is after
    // the specified time step
    bool no_actions_to_start =
        action_pq_.empty() || action_pq_.top()->start_time_step() > time_step;

    // `startint_pq_` is empty or the end time of the very first action is after
    // the specified time step
    bool no_actions_to_take_effect =
        starting_action_pq_.empty() ||
        starting_action_pq_.top()->end_time_step() > time_step;

    if (no_actions_to_start && no_actions_to_take_effect) {
      // no actions should be pushed or poped at this time step
      break;
    }

    // check whose top action of the two PQ goes first
    if (!starting_action_pq_.empty() &&
        (action_pq_.empty() || starting_action_pq_.top()->end_time_step() <=
                                   action_pq_.top()->start_time_step())) {
      // pop the action in `starting_action_pq_`
      const auto action = starting_action_pq_.top();
      starting_action_pq_.pop();

      // Simulate this environment before an action starts to be executed
      SimulateToTimeStep(action->end_time_step());

      // TODO: GLOG
      std::cout << "Executed an action at " << action->end_time_step()
                << "th time step\n";
      terrain_.ExecuteAction(*action);
    } else if (!action_pq_.empty()) {
      // pop the action in `action_pq_`
      const auto action = action_pq_.top();
      action_pq_.pop();

      // TODO: GLOG
      std::cout << "Starting an action at " << action->start_time_step()
                << "th time step\n";
      starting_action_pq_.push(action);
    }
  }
}

void Environment::SimulateToTimeStep(const int64_t time_step) {
  // No need to simulate
  if (time_step == time_step_) {
    return;
  }

  int64_t time_step_diff = time_step - time_step_;
  auto new_timestamp = timestamp_ + (time_step_diff * time_step_length_);
  // TODO: GLOG

  while (time_step_ < time_step) 
  {
    time_step_++;

    // Iterate through all plants
    // std::vector<const Plant *> all_plants = terrain_.GetAllPlants();
    // for (std::vector<const Plant *>::iterator it = terrain_.GetAllPlants().begin(); it != all_plants.end(); ++it) 
    // {
      // const Plant* plant = (*it);


    // }

    // daily soil water content (in mm day-1):
    // double dPotE = dETs * 1000.0 / (2454000.0 * 998.0);
    // double dPotT = dETc * 1000.0 / (2454000.0 * 998.0);
    // DailyWaterContent(dPotE, dPotT);
  }

  // TODO: call all other simulators

  // Meteorology simulator

  // Water simulator
  // Photosynthesis simulator
  // Energy balance simulator
  // plant radiation simulator

  // TODO: plant growth model
  

  // Update the information of environment

  time_step_ = time_step;
  timestamp_ = new_timestamp;
}

std::ostream &operator<<(std::ostream &os, const Environment &env) {
  auto c_timestamp = std::chrono::system_clock::to_time_t(env.timestamp_);
  os << std::ctime(&c_timestamp);
  os << env.time_step_ << "th time step\n";
  os << env.weather_;
  os << env.terrain_;

  return os;
}

}  // namespace environment
