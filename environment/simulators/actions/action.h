#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_ACTION_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_ACTION_H_

#include <chrono>
#include <utility>
#include <vector>

#include "simulators/resource.h"
#include "terrain.h"

namespace simulator {

namespace action {

// This defines the types of actions that an agent/human can take.
enum ActionType { CROP_ADD = 0, CROP_REMOVE, CROP_HARVEST,
              COUNT
              };

// An agent or a human can set up this `Action` object and pass it to the
// environment to schedule the action.
// The simulator will then execute this action on the scheduled time.
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

  // This should directly call its corresponding function in `struct
  // ActionExecutorList`. We should not write anything functional here since
  // this is not a friend of the `environment::Environment`.
  virtual void Execute(environment::Terrain* terrain) const = 0;

  const ActionType type;

  const std::vector<environment::Coordinate> applied_range;
  const std::chrono::system_clock::time_point start_time;
  const std::chrono::duration<int> duration;
  const std::vector<std::pair<ResourceType, size_t>> cost;
};

using ActionList = std::vector<const Action*>;

// Custom comparator for `struct Action`
// These two custom comparators are used for the priority queues in the
// `MainSimulator`
struct ActionStartTimeComparator {
  bool operator()(const Action* const lhs, const Action* const rhs) const;
};

struct ActionEndTimeComparator {
  bool operator()(const Action* const lhs, const Action* const rhs) const;
};

// forward declaration all actions
namespace crop {
struct Add;
struct Remove;
struct Harvest;
}  // namespace crop

}  // namespace action

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_ACTION_H_