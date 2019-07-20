#include "main_simulator.h"

#include <iostream>

#include "environment.h"

namespace simulator {

MainSimulator::MainSimulator() : action_pq_(), sun_simulator_() {}

void MainSimulator::SimulateToTime(
    environment::Environment *env,
    const std::chrono::system_clock::time_point &time) {
  int64_t time_step =
      env->time_step() + ((time - env->timestamp()) / env->time_step_length());
  // run the sun simulator
  sun_simulator_.SimulateToTime(env, time);

  // collect actions which should have started before the specific time
  while (!action_pq_.empty() &&
         action_pq_.top()->start_time_step() < time_step) {
    auto action = action_pq_.top();
    action_pq_.pop();
    pending_action_pq_.push(action);
  }

  // really execute actions which should have completed before the specific time
  while (!pending_action_pq_.empty() &&
         (pending_action_pq_.top()->start_time_step() +
          pending_action_pq_.top()->duration()) < time_step) {
    auto action = pending_action_pq_.top();
    pending_action_pq_.pop();

    // TODO: call all other simulators to sync the time
    // e.g.
    // dumb_simulator.SimulateToTime(env, time);

    action->Execute(&(env->terrain_));
  }
}

void MainSimulator::ReceiveActions(
    const std::vector<const agent::action::Action *> &actions) {
  for (const auto &action : actions) {
    action_pq_.push(action);
  }
}

}  // namespace simulator