#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_CROP_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_CROP_H_

#include <chrono>
#include <string>

#include "action.h"
#include "plant_types/plant_type.h"

namespace simulator {

namespace action {

namespace crop {

// Plant a crop
struct Add : public Action {
  const std::string crop_type_name;

  Add(const environment::Coordinate& target,
      const std::chrono::system_clock::time_point& start_time,
      const std::chrono::duration<int>& duration,
      const std::string& crop_type_name);

  Add(const std::vector<environment::Coordinate>& applied_range,
      const std::chrono::system_clock::time_point& start_time,
      const std::chrono::duration<int>& duration,
      const std::string& crop_type_name);

  Add(const environment::Coordinate& target,
      const std::chrono::system_clock::time_point& start_time,
      const std::chrono::duration<int>& duration,
      const std::string& crop_type_name,
      const std::vector<std::pair<ResourceType, size_t>>& cost);

  Add(const std::vector<environment::Coordinate>& applied_range,
      const std::chrono::system_clock::time_point& start_time,
      const std::chrono::duration<int>& duration,
      const std::string& crop_type_name,
      const std::vector<std::pair<ResourceType, size_t>>& cost);

  void Execute(environment::Terrain* terrain) const override;
};

// Remove a crop
struct Remove : public Action {
    Remove(const environment::Coordinate& target,
        const std::chrono::system_clock::time_point& start_time,
        const std::chrono::duration<int>& duration);

    Remove(const std::vector<environment::Coordinate>& applied_range,
        const std::chrono::system_clock::time_point& start_time,
        const std::chrono::duration<int>& duration);

    Remove(const environment::Coordinate& target,
        const std::chrono::system_clock::time_point& start_time,
        const std::chrono::duration<int>& duration,
        const std::vector<std::pair<ResourceType, size_t>>& cost);

    Remove(const std::vector<environment::Coordinate>& applied_range,
        const std::chrono::system_clock::time_point& start_time,
        const std::chrono::duration<int>& duration,
        const std::vector<std::pair<ResourceType, size_t>>& cost);

    void Execute(environment::Terrain* terrain) const override;
};

}  // namespace crop

}  // namespace action

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATOR_ACTIONS_CROP_H_