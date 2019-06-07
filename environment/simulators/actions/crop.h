#ifndef COMPUTATIONAL_AGROECOLOGY_SIMULATOR_ACTIONS_CROP_H_
#define COMPUTATIONAL_AGROECOLOGY_SIMULATOR_ACTIONS_CROP_H_

#include <chrono>

#include "action.h"
#include "plant_types/plant_type.h"

namespace simulator {

namespace action {

namespace crop {

struct Add : public Action {
  const environment::plant_type::TypeId crop_type;

  Add(const environment::Coordinate& target,
      const std::chrono::system_clock::time_point& start_time,
      const std::chrono::duration<int>& duration,
      const environment::plant_type::TypeId crop_type);

  Add(const std::vector<environment::Coordinate>& applied_range,
      const std::chrono::system_clock::time_point& start_time,
      const std::chrono::duration<int>& duration,
      const environment::plant_type::TypeId crop_type);

  Add(const environment::Coordinate& target,
      const std::chrono::system_clock::time_point& start_time,
      const std::chrono::duration<int>& duration,
      const environment::plant_type::TypeId crop_type,
      const std::vector<std::pair<ResourceType, size_t>>& cost);

  Add(const std::vector<environment::Coordinate>& applied_range,
      const std::chrono::system_clock::time_point& start_time,
      const std::chrono::duration<int>& duration,
      const environment::plant_type::TypeId crop_type,
      const std::vector<std::pair<ResourceType, size_t>>& cost);

  void Execute(environment::Terrain* terrain) const override;
};

}  // namespace crop

}  // namespace action

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_SIMULATOR_ACTIONS_CROP_H_