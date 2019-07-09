#include "agent.h"

#include "environment/plant.h"

namespace agent {

Agent::Agent(const std::string &name, environment::Environment *env)
    : name_(name), env_(env), owned_resources_() {}

Agent::Agent(const std::string &name, environment::Environment *env,
             const Resources &owned_resources)
    : name_(name), env_(env), owned_resources_(owned_resources) {}

void Agent::AddResource(const agent::ResourceType &resource, size_t quantity) {
  owned_resources_[resource] += quantity;
}

int Agent::RandomInt(int min, int max) {
  // obtain a random number from hardware
  std::random_device ramdom_number;
  // seed the generator
  std::mt19937 seed(ramdom_number());
  std::uniform_int_distribution<> integer_distributor(min, max);
  return integer_distributor(seed);
}

agent::action::Action *Agent::CreateAction(const ActionID &action) {
  agent::action::Action *new_action;
  // Create a action at time env_->timestamp()
  using ::agent::action::ActionType;
  switch (action.action_taken) {
    case ActionType::CROP_ADD:
      new_action = new agent::action::crop::Add(
          environment::Coordinate(action.row, action.col), env_->time_step(), 1,
          kCornTypeName);
      break;
    case ActionType::WATER_CROP:
      new_action = new agent::action::crop::Water(
          environment::Coordinate(action.row, action.col), env_->time_step(), 1,
          1);
      break;
    case ActionType::CROP_HARVEST:
      new_action = new agent::action::crop::Harvest(
          environment::Coordinate(action.row, action.col), env_->time_step(),
          1);
      break;
  }
  // Remember to delete action
  return new_action;
}
Agent::ReturnCodes Agent::TakeAction(const agent::action::Action *action) {
  if (action == nullptr) {
    return INVALID_ARGUMENT;
  }

  if (!CheckEnoughResources(action->cost())) {
    return NOT_ENOUGH_RESOURCES;
  }

  env_->ReceiveAction(action);
  DeductResources(action->cost());

  return SUCCESS;
}

std::vector<std::string> Agent::GetQualifiedPlants() {
  std::vector<std::string> qualified_plants;

  // TODO: This needs to be refactored.
  // for (const auto& plant_type : environment::plant_type::plant_type_to_plant)
  // {
  //   bool qualified = true;

  //   if (plant_type.second->absolute_temperature.has_value()) {
  //     qualified &= (plant_type.second->absolute_temperature->max >=
  //                   env_->climate().yearly_temperature.max);
  //     qualified &= (plant_type.second->absolute_temperature->min <=
  //                   env_->climate().yearly_temperature.min);
  //   }

  //   if (plant_type.second->absolute_annual_rainfall.has_value()) {
  //     qualified &= (plant_type.second->absolute_annual_rainfall->max >=
  //                   env_->climate().yearly_rainfall.max);
  //     qualified &= (plant_type.second->absolute_annual_rainfall->min <=
  //                   env_->climate().yearly_rainfall.min);
  //   }

  //   if (qualified) {
  //     qualified_plants.push_back(plant_type.first);
  //   }
  // }

  return qualified_plants;
}

bool Agent::CheckEnoughResources(const Resources &resources) const {
  bool ret = true;
  for (const auto &resource : resources) {
    const auto &find_result = owned_resources_.find(resource.first);
    if (resource.second > 0 && find_result == owned_resources_.end()) {
      return false;
    }
    ret &= (find_result->second >= resource.second);
  }

  return ret;
}

void Agent::DeductResources(const Resources &cost) {
  for (const auto &resource : cost) {
    owned_resources_[resource.first] -= resource.second;
  }
}

}  // namespace agent