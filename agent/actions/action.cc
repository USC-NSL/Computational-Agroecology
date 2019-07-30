#include "action.h"

#include "environment/environment.h"

namespace agent {

namespace action {

bool Action::operator==(const Action &rhs) const {
  return (type_ == rhs.type_) && (applied_range_ == rhs.applied_range_) &&
         (start_time_step_ == rhs.start_time_step_) &&
         (duration_ == rhs.duration_) && (cost_ == rhs.cost_);
}

Action::Action(const agent::action::ActionType type,
               const environment::Coordinate &target,
               const int64_t &start_time_step, const int64_t &duration)
    : type_(type),
      applied_range_(std::vector<environment::Coordinate>(1, target)),
      start_time_step_(start_time_step),
      duration_(duration),
      cost_(agent::Resources()) {}

Action::Action(const agent::action::ActionType type,
               const std::vector<environment::Coordinate> &applied_range,
               const int64_t &start_time_step, const int64_t &duration)
    : type_(type),
      applied_range_(applied_range),
      start_time_step_(start_time_step),
      duration_(duration),
      cost_(agent::Resources()) {}

Action::Action(const agent::action::ActionType type,
               const environment::Coordinate &target,
               const int64_t &start_time_step, const int64_t &duration,
               const agent::Resources &cost)
    : type_(type),
      applied_range_(std::vector<environment::Coordinate>(1, target)),
      start_time_step_(start_time_step),
      duration_(duration),
      cost_(cost) {}

Action::Action(const agent::action::ActionType type,
               const std::vector<environment::Coordinate> &applied_range,
               const int64_t &start_time_step, const int64_t &duration,
               const agent::Resources &cost)
    : type_(type),
      applied_range_(applied_range),
      start_time_step_(start_time_step),
      duration_(duration),
      cost_(cost) {}

bool ActionStartTimeComparator::operator()(const Action *const lhs,
                                           const Action *const rhs) const {
  return lhs->start_time_step() > rhs->start_time_step();
}

bool ActionEndTimeComparator::operator()(const Action *const lhs,
                                         const Action *const rhs) const {
  return lhs->end_time_step() > rhs->end_time_step();
}

}  // namespace action

}  // namespace agent