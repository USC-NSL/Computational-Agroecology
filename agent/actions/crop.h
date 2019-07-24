#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_ACTIONS_CROP_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_ACTIONS_CROP_H_

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

#include "agent/actions/action.h"
#include "environment/plant.h"

namespace agent {

namespace action {

namespace crop {

// Plant a crop
class Add : public Action {
 public:
  Add(const environment::Coordinate &target, const int64_t &start_time_step,
      const int64_t &duration, const std::string &crop_type_name);

  Add(const std::vector<environment::Coordinate> &applied_range,
      const int64_t &start_time_step, const int64_t &duration,
      const std::string &crop_type_name);

  Add(const environment::Coordinate &target, const int64_t &start_time_step,
      const int64_t &duration, const agent::Resources &cost,
      const std::string &crop_type_name);

  Add(const std::vector<environment::Coordinate> &applied_range,
      const int64_t &start_time_step, const int64_t &duration,
      const agent::Resources &cost, const std::string &crop_type_name);

  void Execute(environment::Terrain *terrain) const override;

  bool operator==(const Add &rhs) const;

 private:
  const std::string crop_type_name_;
};

// Remove a crop
class Remove : public Action {
 public:
  Remove(const environment::Coordinate &target, const int64_t &start_time_step,
         const int64_t &duration);

  Remove(const std::vector<environment::Coordinate> &applied_range,
         const int64_t &start_time_step, const int64_t &duration);

  Remove(const environment::Coordinate &target, const int64_t &start_time_step,
         const int64_t &duration, const agent::Resources &cost);

  Remove(const std::vector<environment::Coordinate> &applied_range,
         const int64_t &start_time_step, const int64_t &duration,
         const agent::Resources &cost);

  void Execute(environment::Terrain *terrain) const override;
};

// Harvest a crop
class Harvest : public Action {
 public:
  Harvest(const environment::Coordinate &target, const int64_t &start_time_step,
          const int64_t &duration);

  Harvest(const std::vector<environment::Coordinate> &applied_range,
          const int64_t &start_time_step, const int64_t &duration);

  Harvest(const environment::Coordinate &target, const int64_t &start_time_step,
          const int64_t &duration, const agent::Resources &cost);

  Harvest(const std::vector<environment::Coordinate> &applied_range,
          const int64_t &start_time_step, const int64_t &duration,
          const agent::Resources &cost);

  void Execute(environment::Terrain *terrain) const override;
};

// Water a crop
class Water : public Action {
 public:
  Water(const environment::Coordinate &target, const int64_t &start_time_step,
        const int64_t &duration, const double &water_amount);

  Water(const std::vector<environment::Coordinate> &applied_range,
        const int64_t &start_time_step, const int64_t &duration,
        const double &water_amount);

  Water(const environment::Coordinate &target, const int64_t &start_time_step,
        const int64_t &duration, const agent::Resources &cost,
        const double &water_amount);

  Water(const std::vector<environment::Coordinate> &applied_range,
        const int64_t &start_time_step, const int64_t &duration,
        const agent::Resources &cost, const double &water_amount);

  void Execute(environment::Terrain *terrain) const override;

  bool operator==(const Water &rhs) const;

 private:
  const double water_amount_;
};

}  // namespace crop

}  // namespace action

}  // namespace agent

#endif  // COMPUTATIONAL_AGROECOLOGY_AGENT_ACTIONS_CROP_H_
