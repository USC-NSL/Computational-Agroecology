#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_MAIN_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_MAIN_SIMULATOR_H_

#include <queue>

#include "actions/action.h"
#include "simulator.h"

namespace simulator {

// The main simulator should be in charge of coordinate all the simulators and
// help agent/human schedule the actions they want to take.
// This should be the only simulator in the `environment::Environment` object.
class MainSimulator : public Simulator {
 public:
  // Constructor
  MainSimulator();

  void SimulateToTime(
      environment::Environment* env,
      const std::chrono::system_clock::time_point& time) override;

  // Receive actions from `environment::Environment`
  void ReceiveActions(const std::vector<const action::Action*>& actions);

  // Accessors
  inline const std::priority_queue<const action::Action*,
                                   std::vector<const action::Action*>,
                                   action::ActionStartTimeComparator>&
  action_pq() const {
    return action_pq_;
  }

  inline const std::priority_queue<const action::Action*,
                                   std::vector<const action::Action*>,
                                   action::ActionEndTimeComparator>&
  pending_action_pq() const {
    return pending_action_pq_;
  }

 private:
  std::priority_queue<const action::Action*, std::vector<const action::Action*>,
                      action::ActionStartTimeComparator>
      action_pq_;
  std::priority_queue<const action::Action*, std::vector<const action::Action*>,
                      action::ActionEndTimeComparator>
      pending_action_pq_;
};

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_MAIN_SIMULATOR_H_