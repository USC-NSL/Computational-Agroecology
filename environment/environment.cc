#include "environment.h"

#include <ctime>

namespace environment {

Environment::Environment(const Config& config,
                         const std::chrono::system_clock::time_point& time,
                         const Terrain& terrain)
    : config_(config),
      climate_(config),
      timestamp_(time),
      terrain_(terrain),
      weather_(climate_, time),
      main_simulator_() {}

void Environment::JumpToTime(
    const std::chrono::system_clock::time_point& time) {
  main_simulator_.SimulateToTime(this, time);
  timestamp_ = time;
}

void Environment::JumpDuration(const std::chrono::duration<int>& duration) {
  JumpToTime(timestamp_ + duration);
}

void Environment::ReceiveAction(const simulator::action::Action* action) {
  ReceiveActions(std::vector<const simulator::action::Action*>(1, action));
}

void Environment::ReceiveActions(
    const std::vector<const simulator::action::Action*>& actions) {
  main_simulator_.ReceiveActions(actions);
}

std::ostream& operator<<(std::ostream& os, const Environment& env) {
  auto c_timestamp = std::chrono::system_clock::to_time_t(env.timestamp_);
  os << std::ctime(&c_timestamp) << '\n';
  os << env.weather_;
  os << env.terrain_;

  return os;
}

}  // namespace environment