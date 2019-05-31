#ifndef COMPUTATIONAL_AGROECOLOGY_SIMULATORS_MAIN_H_
#define COMPUTATIONAL_AGROECOLOGY_SIMULATORS_MAIN_H_

#include <queue>

#include "actions/action.h"
#include "simulator.h"

namespace simulator {

class MainSimulator : public Simulator {
 public:
  MainSimulator();

  void SimulateToTime(
      environment::Environment* env,
      const std::chrono::system_clock::time_point& time) override;

  void ReceiveActions(const std::vector<const action::Action*>& actions);

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

#endif  // COMPUTATIONAL_AGROECOLOGY_SIMULATORS_MAIN_H_