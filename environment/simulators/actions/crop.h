#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_CROP_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_CROP_H_

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

#include "action.h"
#include "plant.h"

namespace simulator {

namespace action {

namespace crop {

// Plant a crop
class Add : public Action {
 public:
  Add(const environment::Coordinate &target,
      const std::chrono::system_clock::time_point &start_time,
      const std::chrono::duration<int> &duration,
      const std::string &crop_type_name);

  Add(const std::vector<environment::Coordinate> &applied_range,
      const std::chrono::system_clock::time_point &start_time,
      const std::chrono::duration<int> &duration,
      const std::string &crop_type_name);

  Add(const environment::Coordinate &target,
      const std::chrono::system_clock::time_point &start_time,
      const std::chrono::duration<int> &duration,
      const std::unordered_map<ResourceType, size_t> &cost,
      const std::string &crop_type_name);

  Add(const std::vector<environment::Coordinate> &applied_range,
      const std::chrono::system_clock::time_point &start_time,
      const std::chrono::duration<int> &duration,
      const std::unordered_map<ResourceType, size_t> &cost,
      const std::string &crop_type_name);

  void Execute(environment::Terrain *terrain) const override;

  bool operator==(const Add &rhs) const;

 private:
  const std::string crop_type_name_;
};

// Remove a crop
class Remove : public Action {
 public:
  Remove(const environment::Coordinate &target,
         const std::chrono::system_clock::time_point &start_time,
         const std::chrono::duration<int> &duration);

  Remove(const std::vector<environment::Coordinate> &applied_range,
         const std::chrono::system_clock::time_point &start_time,
         const std::chrono::duration<int> &duration);

  Remove(const environment::Coordinate &target,
         const std::chrono::system_clock::time_point &start_time,
         const std::chrono::duration<int> &duration,
         const std::unordered_map<ResourceType, size_t> &cost);

  Remove(const std::vector<environment::Coordinate> &applied_range,
         const std::chrono::system_clock::time_point &start_time,
         const std::chrono::duration<int> &duration,
         const std::unordered_map<ResourceType, size_t> &cost);

  void Execute(environment::Terrain *terrain) const override;
};

// Harvest a crop
class Harvest : public Action {
 public:
  Harvest(const environment::Coordinate &target,
          const std::chrono::system_clock::time_point &start_time,
          const std::chrono::duration<int> &duration);

  Harvest(const std::vector<environment::Coordinate> &applied_range,
          const std::chrono::system_clock::time_point &start_time,
          const std::chrono::duration<int> &duration);

  Harvest(const environment::Coordinate &target,
          const std::chrono::system_clock::time_point &start_time,
          const std::chrono::duration<int> &duration,
          const std::unordered_map<ResourceType, size_t> &cost);

  Harvest(const std::vector<environment::Coordinate> &applied_range,
          const std::chrono::system_clock::time_point &start_time,
          const std::chrono::duration<int> &duration,
          const std::unordered_map<ResourceType, size_t> &cost);

  void Execute(environment::Terrain *terrain) const override;
};

// Water a crop
class Water : public Action {
 public:
  Water(const environment::Coordinate &target,
        const std::chrono::system_clock::time_point &start_time,
        const std::chrono::duration<int> &duration, const double &water_amount);

  Water(const std::vector<environment::Coordinate> &applied_range,
        const std::chrono::system_clock::time_point &start_time,
        const std::chrono::duration<int> &duration, const double &water_amount);

  Water(const environment::Coordinate &target,
        const std::chrono::system_clock::time_point &start_time,
        const std::chrono::duration<int> &duration,
        const std::unordered_map<ResourceType, size_t> &cost,
        const double &water_amount);

  Water(const std::vector<environment::Coordinate> &applied_range,
        const std::chrono::system_clock::time_point &start_time,
        const std::chrono::duration<int> &duration,
        const std::unordered_map<ResourceType, size_t> &cost,
        const double &water_amount);

  void Execute(environment::Terrain *terrain) const override;

  bool operator==(const Water &rhs) const;

 private:
  const double water_amount_;
};

}  // namespace crop

}  // namespace action

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_CROP_H_
