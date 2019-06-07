#ifndef COMPUTATIONAL_AGROECOLOGY_SIMULATOR_ACTIONS_ACTION_H_
#define COMPUTATIONAL_AGROECOLOGY_SIMULATOR_ACTIONS_ACTION_H_

#include <chrono>
#include <utility>
#include <vector>

#include "simulators/resource.h"
#include "terrain.h"

namespace simulator {

namespace action {

enum ActionType { CROP_ADD = 0 };

struct Action {
  Action(const ActionType type, const environment::Coordinate& target,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration);

  Action(const ActionType type,
         const std::vector<environment::Coordinate>& applied_range,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration);

  Action(const ActionType type, const environment::Coordinate& target,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const std::vector<std::pair<ResourceType, size_t>>& cost);

  Action(const ActionType type,
         const std::vector<environment::Coordinate>& applied_range,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const std::vector<std::pair<ResourceType, size_t>>& cost);
  const ActionType type;

  const std::vector<environment::Coordinate> applied_range;
  const std::chrono::system_clock::time_point start_time;
  const std::chrono::duration<int> duration;
  const std::vector<std::pair<ResourceType, size_t>> cost;

  virtual void Execute(environment::Terrain* terrain) const = 0;
};

// Just declarations here
struct ActionList {
  static void CROP_ADD_execute(environment::Terrain* const terrain,
                               const Action* action);
};

// Custom comparator for `struct Action`
struct ActionStartTimeComparator {
  bool operator()(const Action* const lhs, const Action* const rhs) const;
};

struct ActionEndTimeComparator {
  bool operator()(const Action* const lhs, const Action* const rhs) const;
};

}  // namespace action

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_SIMULATOR_ACTIONS_ACTION_H_