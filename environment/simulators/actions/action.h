#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_ACTION_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_ACTION_H_

#include <chrono>
#include <unordered_map>
#include <utility>
#include <vector>

#include "agent/resource.h"
#include "terrain.h"

namespace simulator {

namespace action {

// This defines the types of actions that an agent/human can take.
enum ActionType { CROP_ADD = 0, CROP_REMOVE, CROP_HARVEST, WATER_CROP, NUM_ACTIONS };

// An agent or a human can set up this `Action` object and pass it to the
// environment to schedule the action.
// The simulator will then execute this action on the scheduled time.
class Action {
 public:
  // execute an action on a specified terrain
  virtual void Execute(environment::Terrain *terrain) const = 0;

  // accessors
  const std::vector<environment::Coordinate> &applied_range() const {
    return applied_range_;
  }
  const int64_t start_time_step() const { return start_time_step_; }
  const int64_t duration() const { return duration_; }
  const agent::Resources &cost() const { return cost_; }

  // operator
  bool operator==(const Action &rhs) const;

 protected:
  // only children can call constructors
  Action(const ActionType type, const environment::Coordinate &target,
         const int64_t &start_time_step, const int64_t &duration);

  Action(const ActionType type,
         const std::vector<environment::Coordinate> &applied_range,
         const int64_t &start_time_step, const int64_t &duration);

  Action(const ActionType type, const environment::Coordinate &target,
         const int64_t &start_time_step, const int64_t &duration,
         const agent::Resources &cost);

  Action(const ActionType type,
         const std::vector<environment::Coordinate> &applied_range,
         const int64_t &start_time_step, const int64_t &duration,
         const agent::Resources &cost);

  // private accessor
  const ActionType &type() const { return type_; }

  const std::vector<environment::Coordinate> applied_range_;

  // The expected start time step of this action
  const int64_t start_time_step_;

  // The expected duration in time steps of this action
  const int64_t duration_;
  const agent::Resources cost_;

 private:
  const ActionType type_;
};

using ActionList = std::vector<const Action *>;

// Custom comparator for `struct Action`
// These two custom comparators are used for the priority queues in the
// `MainSimulator`
struct ActionStartTimeComparator {
  bool operator()(const Action *const lhs, const Action *const rhs) const;
};

struct ActionEndTimeComparator {
  bool operator()(const Action *const lhs, const Action *const rhs) const;
};

// forward declaration all actions
namespace crop {
struct Add;
struct Remove;
struct Harvest;
struct Water;
}  // namespace crop

}  // namespace action

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_ACTION_H_