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
      main_simulator_() {
  auto to_round = timestamp_.time_since_epoch() % time_step_length_;
  timestamp_ -= to_round;
}

void Environment::JumpToTimeStep(const int64_t time_step) {
  int64_t time_step_diff = time_step - time_step_;
  JumpForwardTimeStep(time_step_diff);
}

void Environment::JumpForwardTimeStep(const int64_t time_step_num) {
  time_step_ += time_step_num;
  JumpToTime(timestamp_ + (time_step_num * time_step_length_));
}

void Environment::ReceiveAction(const agent::action::Action *action) {
  ReceiveActions(agent::action::ActionList(1, action));
}

void Environment::ReceiveActions(const agent::action::ActionList &actions) {
  main_simulator_.ReceiveActions(actions);
}

void Environment::JumpToTime(
    const std::chrono::system_clock::time_point &time) {
  main_simulator_.SimulateToTime(this, time);
  timestamp_ = time;
}

std::ostream &operator<<(std::ostream &os, const Environment &env) {
  auto c_timestamp = std::chrono::system_clock::to_time_t(env.timestamp_);
  os << std::ctime(&c_timestamp) << '\n';
  os << env.time_step_ << "th time step\n";
  os << env.weather_;
  os << env.terrain_;

  return os;
}

}  // namespace environment