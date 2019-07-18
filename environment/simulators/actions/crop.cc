#include "crop.h"

#include "plant_builder.h"

namespace simulator {

namespace action {

namespace crop {

Add::Add(const environment::Coordinate &target, const int64_t &start_time_step,
         const int64_t &duration, const std::string &crop_type_name)
    : Action(CROP_ADD, target, start_time_step, duration),
      crop_type_name_(crop_type_name) {}

Add::Add(const std::vector<environment::Coordinate> &applied_range,
         const int64_t &start_time_step, const int64_t &duration,
         const std::string &crop_type_name)
    : Action(CROP_ADD, applied_range, start_time_step, duration),
      crop_type_name_(crop_type_name) {}

Add::Add(const environment::Coordinate &target, const int64_t &start_time_step,
         const int64_t &duration, const agent::Resources &cost,
         const std::string &crop_type_name)
    : Action(CROP_ADD, target, start_time_step, duration, cost),
      crop_type_name_(crop_type_name) {}

Add::Add(const std::vector<environment::Coordinate> &applied_range,
         const int64_t &start_time_step, const int64_t &duration,
         const agent::Resources &cost, const std::string &crop_type_name)
    : Action(CROP_ADD, applied_range, start_time_step, duration, cost),
      crop_type_name_(crop_type_name) {}

void Add::Execute(environment::Terrain *terrain) const {
  // Check if the input argument is valid
  if (terrain == nullptr) {
    return;
  }

  // TODO: should be replaced by GLOGS
  std::cout << "Adding " << applied_range_.size() << " crop(s)." << std::endl;

  using environment::PlantBuilder;
  for (const auto &c : applied_range_) {
    terrain->tiles().get(c).plant.reset(
        PlantBuilder::NewPlant(crop_type_name_));
  }
}

bool Add::operator==(const Add &rhs) const {
  using simulator::action::Action;
  const auto &action_lhs = reinterpret_cast<const Action &>(*this);
  const auto &action_rhs = reinterpret_cast<const Action &>(rhs);

  return (action_lhs == action_rhs) && (crop_type_name_ == rhs.crop_type_name_);
}

Remove::Remove(const environment::Coordinate &target,
               const int64_t &start_time_step, const int64_t &duration)
    : Action(CROP_REMOVE, target, start_time_step, duration) {}

Remove::Remove(const std::vector<environment::Coordinate> &applied_range,
               const int64_t &start_time_step, const int64_t &duration)
    : Action(CROP_REMOVE, applied_range, start_time_step, duration) {}

Remove::Remove(const environment::Coordinate &target,
               const int64_t &start_time_step, const int64_t &duration,
               const agent::Resources &cost)
    : Action(CROP_REMOVE, target, start_time_step, duration, cost) {}

Remove::Remove(const std::vector<environment::Coordinate> &applied_range,
               const int64_t &start_time_step, const int64_t &duration,
               const agent::Resources &cost)
    : Action(CROP_REMOVE, applied_range, start_time_step, duration, cost) {}

void Remove::Execute(environment::Terrain *terrain) const {
  // Check if the input argument is valid
  if (terrain == nullptr) {
    return;
  }

  // TODO: should be replaced by GLOGS
  std::cout << "Removing " << applied_range_.size() << " crop(s)." << std::endl;

  for (const auto &c : applied_range_) {
    terrain->tiles().get(c).plant.reset();
  }
}

Harvest::Harvest(const environment::Coordinate &target,
                 const int64_t &start_time_step, const int64_t &duration)
    : Action(CROP_HARVEST, target, start_time_step, duration) {}

Harvest::Harvest(const std::vector<environment::Coordinate> &applied_range,
                 const int64_t &start_time_step, const int64_t &duration)
    : Action(CROP_HARVEST, applied_range, start_time_step, duration) {}

Harvest::Harvest(const environment::Coordinate &target,
                 const int64_t &start_time_step, const int64_t &duration,
                 const agent::Resources &cost)
    : Action(CROP_HARVEST, target, start_time_step, duration, cost) {}

Harvest::Harvest(const std::vector<environment::Coordinate> &applied_range,
                 const int64_t &start_time_step, const int64_t &duration,
                 const agent::Resources &cost)
    : Action(CROP_HARVEST, applied_range, start_time_step, duration, cost) {}

void Harvest::Execute(environment::Terrain *terrain) const {
  // Check if the input argument is valid
  if (terrain == nullptr) {
    return;
  }

  // TODO: should be replaced by GLOGS
  std::cout << "Harvesting crop from " << applied_range_.size() << " crop(s)."
            << std::endl;

  for (const auto &c : applied_range_) {
    if (terrain->tiles().get(c).plant != nullptr) {
      int yield = terrain->tiles().get(c).plant->Harvest();
      terrain->AddYield(yield);
    }
  }
  std::cout << "Yield of terrain: " << terrain->yield() << "kg." << std::endl;
}

Water::Water(const environment::Coordinate &target,
             const int64_t &start_time_step, const int64_t &duration,
             const double &water_amount)
    : Action(WATER_CROP, target, start_time_step, duration),
      water_amount_(water_amount) {}

Water::Water(const std::vector<environment::Coordinate> &applied_range,
             const int64_t &start_time_step, const int64_t &duration,
             const double &water_amount)
    : Action(WATER_CROP, applied_range, start_time_step, duration),
      water_amount_(water_amount) {}

Water::Water(const environment::Coordinate &target,
             const int64_t &start_time_step, const int64_t &duration,
             const agent::Resources &cost, const double &water_amount)
    : Action(WATER_CROP, target, start_time_step, duration, cost),
      water_amount_(water_amount) {}

Water::Water(const std::vector<environment::Coordinate> &applied_range,
             const int64_t &start_time_step, const int64_t &duration,
             const agent::Resources &cost, const double &water_amount)
    : Action(WATER_CROP, applied_range, start_time_step, duration, cost),
      water_amount_(water_amount) {}

void Water::Execute(environment::Terrain *terrain) const {
  // Check if the input argument is valid
  if (terrain == nullptr) {
    return;
  }

  for (const auto &c : applied_range_) {
    terrain->tiles().get(c).soil.water_content += water_amount_;
  }
}

bool Water::operator==(const Water &rhs) const {
  using simulator::action::Action;
  const auto &action_lhs = reinterpret_cast<const Action &>(*this);
  const auto &action_rhs = reinterpret_cast<const Action &>(rhs);

  return (action_lhs == action_rhs) && (water_amount_ == rhs.water_amount_);
}

}  // namespace crop

}  // namespace action

}  // namespace simulator