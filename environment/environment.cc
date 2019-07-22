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
      weather_(climate_, time) {
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
  // Go forward in the timeline. When an action should be done in this time
  // step, execute it. When an action starts, put it into the
  // `pending_action_pq`.
  // Extract one action per loop.
  while (!action_pq_.empty() || !pending_action_pq_.empty()) {
    // no actions should be extract at this time step
    bool no_need_to_mark_an_action_started =
        action_pq_.empty() || action_pq_.top()->start_time_step() > time_step;
    bool no_need_to_execute_an_action_from_pending =
        pending_action_pq_.empty() ||
        pending_action_pq_.top()->end_time_step() > time_step;
    if (no_need_to_mark_an_action_started &&
        no_need_to_execute_an_action_from_pending) {
      break;
    }

    if (!pending_action_pq_.empty() &&
        pending_action_pq_.top()->end_time_step() <=
            action_pq_.top()->start_time_step()) {
      auto action = pending_action_pq_.top();
      pending_action_pq_.pop();

      // Simulate this environment before an action starts to be executed
      SimulateToTimeStep(action->end_time_step());

      // TODO: GLOG
      std::cout << "Executed an action at " << action->end_time_step()
                << "th time step\n";
      terrain_.ExecuteAction(action);
    } else if (!action_pq_.empty()) {
      auto action = action_pq_.top();
      action_pq_.pop();

      // TODO: GLOG
      std::cout << "Starting an action at " << action->start_time_step()
                << "th time step\n";
      pending_action_pq_.push(action);
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
  sun_info_.SimulateToTime(new_timestamp, config_.location);

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