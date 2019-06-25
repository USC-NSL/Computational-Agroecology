#include "crop.h"

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

Water::Water(const environment::Coordinate &target,
             const std::chrono::system_clock::time_point &start_time,
             const std::chrono::duration<int> &duration,
             const double &water_amount)
    : Action(WATER_CROP, target, start_time, duration),
      water_amount(water_amount) {}

Water::Water(const std::vector<environment::Coordinate> &applied_range,
             const std::chrono::system_clock::time_point &start_time,
             const std::chrono::duration<int> &duration,
             const double &water_amount)
    : Action(WATER_CROP, applied_range, start_time, duration),
      water_amount(water_amount) {}

Water::Water(const environment::Coordinate &target,
             const std::chrono::system_clock::time_point &start_time,
             const std::chrono::duration<int> &duration,
             const double &water_amount,
             const std::vector<std::pair<ResourceType, size_t>> &cost)
    : Action(WATER_CROP, target, start_time, duration, cost),
      water_amount(water_amount) {}

Water::Water(const std::vector<environment::Coordinate> &applied_range,
             const std::chrono::system_clock::time_point &start_time,
             const std::chrono::duration<int> &duration,
             const double &water_amount,
             const std::vector<std::pair<ResourceType, size_t>> &cost)
    : Action(WATER_CROP, applied_range, start_time, duration, cost),
      water_amount(water_amount) {}

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

void Water::Execute(environment::Terrain* terrain) const {
  using environment::plant_type::plant_type_to_plant;

  for (const auto& c : applied_range) {
    terrain->tiles().get(c).soil.water_content +=
        water_amount; // random number,
  }
}

bool operator==(const Add& lhs, const Add& rhs) {
  const auto& tmp_lhs = reinterpret_cast<const simulator::action::Action&>(lhs);
  const auto& tmp_rhs = reinterpret_cast<const simulator::action::Action&>(rhs);
  return (tmp_lhs == tmp_rhs) && (lhs.crop_type_name == rhs.crop_type_name);
}

bool operator==(const Remove& lhs, const Remove& rhs) {
  const auto& tmp_lhs = reinterpret_cast<const simulator::action::Action&>(lhs);
  const auto& tmp_rhs = reinterpret_cast<const simulator::action::Action&>(rhs);
  return tmp_lhs == tmp_rhs;
}

bool operator==(const Harvest& lhs, const Harvest& rhs) {
  const auto& tmp_lhs = reinterpret_cast<const simulator::action::Action&>(lhs);
  const auto& tmp_rhs = reinterpret_cast<const simulator::action::Action&>(rhs);
  return tmp_lhs == tmp_rhs;
}

}  // namespace crop

}  // namespace action

}  // namespace simulator