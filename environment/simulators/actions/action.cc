#include "action.h"

#include "environment.h"

namespace simulator {

namespace action {

bool Action::operator==(const Action &rhs) const {
  return (type_ == rhs.type_) && (applied_range_ == rhs.applied_range_) &&
         (start_time_ == rhs.start_time_) && (duration_ == rhs.duration_) &&
         (cost_ == rhs.cost_);
}

Action::Action(const simulator::action::ActionType type,
               const environment::Coordinate &target,
               const std::chrono::system_clock::time_point &start_time,
               const std::chrono::duration<int> &duration)
    : type_(type),
      applied_range_(std::vector<environment::Coordinate>(1, target)),
      start_time_(start_time),
      duration_(duration),
      cost_(std::unordered_map<ResourceType, size_t>()) {}

Action::Action(const simulator::action::ActionType type,
               const std::vector<environment::Coordinate> &applied_range,
               const std::chrono::system_clock::time_point &start_time,
               const std::chrono::duration<int> &duration)
    : type_(type),
      applied_range_(applied_range),
      start_time_(start_time),
      duration_(duration),
      cost_(std::unordered_map<ResourceType, size_t>()) {}

Action::Action(const simulator::action::ActionType type,
               const environment::Coordinate &target,
               const std::chrono::system_clock::time_point &start_time,
               const std::chrono::duration<int> &duration,
               const std::unordered_map<ResourceType, size_t> &cost)
    : type_(type),
      applied_range_(std::vector<environment::Coordinate>(1, target)),
      start_time_(start_time),
      duration_(duration),
      cost_(cost) {}

Action::Action(const simulator::action::ActionType type,
               const std::vector<environment::Coordinate> &applied_range,
               const std::chrono::system_clock::time_point &start_time,
               const std::chrono::duration<int> &duration,
               const std::unordered_map<ResourceType, size_t> &cost)
    : type_(type),
      applied_range_(applied_range),
      start_time_(start_time),
      duration_(duration),
      cost_(cost) {}

bool ActionStartTimeComparator::operator()(const Action *const lhs,
                                           const Action *const rhs) const {
  return lhs->start_time() > rhs->start_time();
}

bool ActionEndTimeComparator::operator()(const Action *const lhs,
                                         const Action *const rhs) const {
  return (lhs->start_time() + lhs->duration()) >
         (rhs->start_time() + rhs->duration());
}

}  // namespace action

}  // namespace simulator