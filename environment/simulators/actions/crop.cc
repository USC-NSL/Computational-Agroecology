#include "crop.h"
#include "plant_types/corn.h"
#include "plant_types/squash.h"
#include "plant_types/bean.h"

namespace simulator {

namespace action {

namespace crop {

Add::Add(const environment::Coordinate& target,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const std::string& crop_type_name)
    : Action(CROP_ADD, target, start_time, duration),
      crop_type_name(crop_type_name) {}

Add::Add(const std::vector<environment::Coordinate>& applied_range,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const std::string& crop_type_name)
    : Action(CROP_ADD, applied_range, start_time, duration),
      crop_type_name(crop_type_name) {}

Add::Add(const environment::Coordinate& target,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const std::string& crop_type_name,
         const std::vector<std::pair<ResourceType, size_t>>& cost)
    : Action(CROP_ADD, target, start_time, duration, cost),
      crop_type_name(crop_type_name) {}

Add::Add(const std::vector<environment::Coordinate>& applied_range,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const std::string& crop_type_name,
         const std::vector<std::pair<ResourceType, size_t>>& cost)
    : Action(CROP_ADD, applied_range, start_time, duration, cost),
      crop_type_name(crop_type_name) {}

Remove::Remove(const environment::Coordinate& target,
               const std::chrono::system_clock::time_point& start_time,
               const std::chrono::duration<int>& duration)
    : Action(CROP_REMOVE, target, start_time, duration) {}

Remove::Remove(const std::vector<environment::Coordinate>& applied_range,
               const std::chrono::system_clock::time_point& start_time,
               const std::chrono::duration<int>& duration)
    : Action(CROP_REMOVE, applied_range, start_time, duration) {}

Remove::Remove(const environment::Coordinate& target,
               const std::chrono::system_clock::time_point& start_time,
               const std::chrono::duration<int>& duration,
               const std::vector<std::pair<ResourceType, size_t>>& cost)
    : Action(CROP_REMOVE, target, start_time, duration, cost) {}

Remove::Remove(const std::vector<environment::Coordinate>& applied_range,
               const std::chrono::system_clock::time_point& start_time,
               const std::chrono::duration<int>& duration,
               const std::vector<std::pair<ResourceType, size_t>>& cost)
    : Action(CROP_REMOVE, applied_range, start_time, duration, cost) {}

Harvest::Harvest(const environment::Coordinate& target,
                 const std::chrono::system_clock::time_point& start_time,
                 const std::chrono::duration<int>& duration)
    : Action(CROP_HARVEST, target, start_time, duration) {}

Harvest::Harvest(const std::vector<environment::Coordinate>& applied_range,
                 const std::chrono::system_clock::time_point& start_time,
                 const std::chrono::duration<int>& duration)
    : Action(CROP_HARVEST, applied_range, start_time, duration) {}

Harvest::Harvest(const environment::Coordinate& target,
                 const std::chrono::system_clock::time_point& start_time,
                 const std::chrono::duration<int>& duration,
                 const std::vector<std::pair<ResourceType, size_t>>& cost)
    : Action(CROP_HARVEST, target, start_time, duration, cost) {}

Harvest::Harvest(const std::vector<environment::Coordinate>& applied_range,
                 const std::chrono::system_clock::time_point& start_time,
                 const std::chrono::duration<int>& duration,
                 const std::vector<std::pair<ResourceType, size_t>>& cost)
    : Action(CROP_HARVEST, applied_range, start_time, duration, cost) {}

void Add::Execute(environment::Terrain* terrain) const {
  using environment::plant_type::plant_type_to_plant;

  // The following standard outputs should be replaced by GLOGS
  std::cout << "Adding " << applied_range.size() << " crop(s)." << std::endl;

  // TODO: fully implement this
  for (const auto& c : applied_range) {
    terrain->tiles().get(c).plant =
        plant_type_to_plant[crop_type_name]->GeneratePlantInstance();
  }
}

void Remove::Execute(environment::Terrain* terrain) const {
  using environment::plant_type::plant_type_to_plant;

  // The following standard outputs should be replaced by GLOGS
  std::cout << "Removing " << applied_range.size() << " crop(s)." << std::endl;

  // TODO: fully implement this
  for (const auto& c : applied_range) {
    terrain->tiles().get(c).plant = std::nullopt;
  }
}

void Harvest::Execute(environment::Terrain* terrain) const {
  using environment::plant_type::plant_type_to_plant;

  // The following standard outputs should be replaced by GLOGS
  std::cout << "Harvesting crop from " << applied_range.size() << " crop(s)."
            << std::endl;

  for (const auto& c : applied_range) {
    if (terrain->tiles().get(c).plant.has_value()) {
      environment::Plant& plant = *(terrain->tiles().get(c).plant);
      terrain->AddYield(plant.produce);
      plant.produce = 0;
    }
  }
  std::cout << "Yield of terrain: " << terrain->yield() << "kg." << std::endl;
}

void init() {
	auto corn = new environment::plant_type::Corn();
	auto bean = new environment::plant_type::Bean();
	auto squash = new environment::plant_type::Squash();
}

}  // namespace crop

}  // namespace action

}  // namespace simulator