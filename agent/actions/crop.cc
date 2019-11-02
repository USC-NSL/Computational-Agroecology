#include "crop.h"

#include "environment/terrain.h"
#include "environment/meteorology.h"

namespace agent {

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
  for (const auto &coordinate : applied_range_) {
    const auto &meteorology = terrain->meteorology();
    terrain->plant_container_.AddPlant(crop_type_name_, coordinate, *meteorology);
  }
}

bool Add::operator==(const Add &rhs) const {
  using agent::action::Action;
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
    terrain->plant_container_.DelPlant(c);
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
    // TODO: generalize the harvest / yield handling
    environment::Plant *plant = terrain->plant_container_.GetPlant(c);
    if (plant != nullptr) {
      int yield = plant->Harvest();
      terrain->yield_ += yield;
      terrain->plant_container_.DelPlant(c);
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
    terrain->soil_container_[c].AddWaterToSoil(water_amount_);
  }
}

bool Water::operator==(const Water &rhs) const {
  using agent::action::Action;
  const auto &action_lhs = reinterpret_cast<const Action &>(*this);
  const auto &action_rhs = reinterpret_cast<const Action &>(rhs);

  return (action_lhs == action_rhs) && (water_amount_ == rhs.water_amount_);
}

}  // namespace crop

}  // namespace action

}  // namespace agent