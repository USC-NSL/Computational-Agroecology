#include "main_simulator.h"

#include <iostream>

#include "environment.h"

namespace simulator {

MainSimulator::MainSimulator() : action_pq_() {}

void MainSimulator::SimulateToTime(
    environment::Environment* env,
    const std::chrono::system_clock::time_point& time) {
  // collect actions which should have started before the specific time
  while (!action_pq_.empty() && action_pq_.top()->start_time < time) {
    auto action = action_pq_.top();
    action_pq_.pop();
    pending_action_pq_.push(action);
  }

  // really execute actions which should have completed before the specific time
  while (!pending_action_pq_.empty() &&
         (pending_action_pq_.top()->start_time +
          pending_action_pq_.top()->duration) < time) {
    auto action = pending_action_pq_.top();
    pending_action_pq_.pop();

    // TODO: call all other simulators to sync the time
    // e.g.
    // dumb_simulator.SimulateToTime(env, time);

    action->Execute(&(env->terrain_));
  }
}

void MainSimulator::ReceiveActions(
    const std::vector<const action::Action*>& actions) {
  for (const auto& action : actions) {
    action_pq_.push(action);
  }
}

}  // namespace simulator