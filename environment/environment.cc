#include "environment.h"

#include <ctime>

namespace environment {

Environment::Environment(const Config &config,
                         const std::chrono::system_clock::time_point &time,
                         const std::chrono::duration<int> &time_step_length,
                         const Terrain &terrain)
    : config_(config),
      climate_(config),
      timestamp_(time),
      time_step_length_(time_step_length),
      time_step_(0),
      terrain_(terrain),
      weather_(climate_, time),
      sun_info_(time, config.location, climate_.climate_zone, weather_) {
  kdtree_ = NULL;
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

  // TODO: call all other simulators
  sun_info_.SimulateToTime(new_timestamp, config_.location,
                           climate_.climate_zone, weather_);

  // Update the information of environment

  time_step_ = time_step;
  timestamp_ = new_timestamp;
}

bool Environment::AddPlant(Plant* new_plant) {
  if (!CheckPosition(new_plant->position(), new_plant->trunk_size()))
    return false;
  plants_.push_back(new_plant);
  ContructPlantKDTree();
}

bool Environment::DelPlant(const int index) {
  delete plants_[index];
  plants_.erase(plants_.begin() + index);
  ContructPlantKDTree();
}

bool Environment::CheckPosition(const point_t &position,
                                const double size) {
  if (!kdtree_)
    return true;
  auto res = kdtree_->neighborhood(position, size);
  if (!res.empty())
    return false;
  else
    return true;
}

void Environment::ContructPlantKDTree() {
  pointVec points;
  for (const auto &plant : plants_) {
    points.push_back(plant->position());
  }
  if (kdtree_)
    delete kdtree_;
  kdtree_ = new KDTree(points);
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