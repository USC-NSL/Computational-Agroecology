#include "crop.h"

#include <iostream>
#include <unordered_map>

#include "plant_types/plant_type.h"

namespace simulator {

namespace action {

void ActionList::CROP_ADD_execute(environment::Terrain* const terrain,
                                  const Action* action) {
  using environment::plant_type::plant_type_to_plant;
  const crop::Add* add_crop = reinterpret_cast<const crop::Add*>(action);

  std::cout << "Adding " << add_crop->applied_range.size() << " crop(s)."
            << std::endl;

  // TODO: fully implement this
  for (const auto& c : action->applied_range) {
    terrain->tiles().get(c).plant =
        plant_type_to_plant[add_crop->crop_type]->GeneratePlantInstance();
  }
}

namespace crop {

Add::Add(const environment::Coordinate& target,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const environment::plant_type::type_id crop_type)
    : Action(CROP_ADD, target, start_time, duration), crop_type(crop_type) {}

Add::Add(const std::vector<environment::Coordinate>& applied_range,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const environment::plant_type::type_id crop_type)
    : Action(CROP_ADD, applied_range, start_time, duration),
      crop_type(crop_type) {}

Add::Add(const environment::Coordinate& target,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const environment::plant_type::type_id crop_type,
         const std::vector<std::pair<resource_type, size_t>>& cost)
    : Action(CROP_ADD, target, start_time, duration, cost),
      crop_type(crop_type) {}

Add::Add(const std::vector<environment::Coordinate>& applied_range,
         const std::chrono::system_clock::time_point& start_time,
         const std::chrono::duration<int>& duration,
         const environment::plant_type::type_id crop_type,
         const std::vector<std::pair<resource_type, size_t>>& cost)
    : Action(CROP_ADD, applied_range, start_time, duration, cost),
      crop_type(crop_type) {}

void Add::Execute(environment::Terrain* terrain) const {
  simulator::action::ActionList::CROP_ADD_execute(terrain, this);
}

}  // namespace crop

}  // namespace action

}  // namespace simulator