#include "agent.h"
#include "plant_types/plant_type.h"

namespace agent {

Agent::Agent(const std::string& name, environment::Environment* env)
    : name_(name), env_(env), owned_resource_() {}

Agent::Agent(
    const std::string& name, environment::Environment* env,
    const std::unordered_map<simulator::ResourceType, size_t>& owned_resource)
    : name_(name), env_(env), owned_resource_(owned_resource) {}

Agent::Agent(const std::string& name, environment::Environment* env,
             const ResourceList& resources_list)
    : Agent(name, env) {
  for (const auto& resource : resources_list) {
    owned_resource_[resource.first] += resource.second;
  }
}

void Agent::AddResource(const simulator::ResourceType& resource,
                        size_t quantity) {
  owned_resource_[resource] += quantity;
}

int Agent::RandomInt(int min, int max){
  // obtain a random number from hardware
  std::random_device ramdom_number; 
  // seed the generator
  std::mt19937 seed(ramdom_number()); 
  std::uniform_int_distribution<> integer_distributor(min, max);
  return integer_distributor(seed);
}

simulator::action::Action* Agent::CreateAction(int terrain_width_ ,int terrain_length_, int action_num, int crop_num){
  const std::string kCornTypeName = "Corn";
  simulator::action::Action* randomAction;

  switch(action_num) {
  case kAdd:
    randomAction = new simulator::action::crop::Add(
    environment::Coordinate(terrain_width_, terrain_length_),
    std::chrono::system_clock::now() + std::chrono::minutes(1),
    std::chrono::duration<int>(), kCornTypeName);
    break;
  case kRemove:
    randomAction = new simulator::action::crop::Remove(
    environment::Coordinate(terrain_width_, terrain_length_),
    std::chrono::system_clock::now() + std::chrono::minutes(1),
    std::chrono::duration<int>());
    break;
  case kHarvest:
    randomAction = new simulator::action::crop::Harvest(
    environment::Coordinate(terrain_width_, terrain_length_),
    std::chrono::system_clock::now() + std::chrono::minutes(1),
    std::chrono::duration<int>());
    break;
  }

  return randomAction;
}
 


Agent::ReturnCodes Agent::TakeAction(const simulator::action::Action* action) {
  if (action == nullptr) {
    return INVALID_ARGUMENT;
  }

  if (!CheckEnoughResources(action->cost)) {
    return NOT_ENOUGH_RESOURCES;
  }

  env_->ReceiveAction(action);
  DeductResources(action->cost);

  return SUCCESS;
}

std::vector<std::string> Agent::GetQualifiedPlants() {
  std::vector<std::string> qualified_plants;

  for (const auto& plant_type : environment::plant_type::plant_type_to_plant) {
    if (plant_type.second->absolute_temperature.max >=
            env_->climate().yearly_temperature.max &&
        plant_type.second->absolute_temperature.min <=
            env_->climate().yearly_temperature.min &&
        plant_type.second->absolute_annual_rainfall.max >=
            env_->climate().yearly_rainfall.max &&
        plant_type.second->absolute_annual_rainfall.min <=
            env_->climate().yearly_rainfall.min) {
      qualified_plants.push_back(plant_type.first);
    }
  }

  return qualified_plants;
}

std::vector<std::string> Agent::GetOptimalPlants() {
  std::vector<std::string> optimal_plants;

  for (const auto& plant_type : environment::plant_type::plant_type_to_plant) {
    if (plant_type.second->optimal_temperature.max >=
            env_->climate().yearly_temperature.max &&
        plant_type.second->optimal_temperature.min <=
            env_->climate().yearly_temperature.min &&
        plant_type.second->optimal_annual_rainfall.max >=
            env_->climate().yearly_rainfall.max &&
        plant_type.second->optimal_annual_rainfall.min <=
            env_->climate().yearly_rainfall.min) {
      optimal_plants.push_back(plant_type.first);
    }
  }

  return optimal_plants;
}

bool Agent::CheckEnoughResources(const ResourceList& resources) const {
  bool ret = true;
  for (const auto& resource : resources) {
    const auto& find_result = owned_resource_.find(resource.first);
    if (resource.second > 0 && find_result == owned_resource_.end()) {
      return false;
    }
    ret &= (find_result->second >= resource.second);
  }

  return ret;
}

void Agent::DeductResources(const ResourceList& cost) {
  for (const auto& resource : cost) {
    owned_resource_[resource.first] -= resource.second;
  }
}

}  // namespace agent