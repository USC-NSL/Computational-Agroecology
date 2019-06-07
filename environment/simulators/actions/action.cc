#include "action.h"

#include "environment.h"

namespace simulator {

namespace action {

Action::Action(const simulator::action::ActionType type,
               const environment::Coordinate& target,
               const std::chrono::system_clock::time_point& start_time,
               const std::chrono::duration<int>& duration)
    : type(type),
      applied_range(std::vector<environment::Coordinate>(1, target)),
      start_time(start_time),
      duration(duration),
      cost(std::vector<std::pair<ResourceType, size_t>>()) {}

Action::Action(const simulator::action::ActionType type,
               const std::vector<environment::Coordinate>& applied_range,
               const std::chrono::system_clock::time_point& start_time,
               const std::chrono::duration<int>& duration)
    : type(type),
      applied_range(applied_range),
      start_time(start_time),
      duration(duration),
      cost(std::vector<std::pair<ResourceType, size_t>>()) {}

Action::Action(const simulator::action::ActionType type,
               const environment::Coordinate& target,
               const std::chrono::system_clock::time_point& start_time,
               const std::chrono::duration<int>& duration,
               const std::vector<std::pair<ResourceType, size_t>>& cost)
    : type(type),
      applied_range(std::vector<environment::Coordinate>(1, target)),
      start_time(start_time),
      duration(duration),
      cost(cost) {}

Action::Action(const simulator::action::ActionType type,
               const std::vector<environment::Coordinate>& applied_range,
               const std::chrono::system_clock::time_point& start_time,
               const std::chrono::duration<int>& duration,
               const std::vector<std::pair<ResourceType, size_t>>& cost)
    : type(type),
      applied_range(applied_range),
      start_time(start_time),
      duration(duration),
      cost(cost) {}

bool ActionStartTimeComparator::operator()(const Action* const lhs,
                                           const Action* const rhs) const {
  return lhs->start_time > rhs->start_time;
}

bool ActionEndTimeComparator::operator()(const Action* const lhs,
                                         const Action* const rhs) const {
  return (lhs->start_time + lhs->duration) > (rhs->start_time + rhs->duration);
}

}  // namespace action

}  // namespace simulator