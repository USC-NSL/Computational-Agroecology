#include "agent.h"

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

Agent::ReturnCodes Agent::TakeAction(const simulator::action::Action* action) {
  if (action == nullptr) {
    return INVALID_ARGUMENT;
  }

  if (!CheckEnoughResources(action->cost)) {
    return NOT_ENOUGH_RESOURCES;
  }

  DeduceResources(action->cost);
  env_->ReceiveAction(action);

  return SUCCESS;
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

void Agent::DeduceResources(const ResourceList& cost) {
  for (const auto& resource : cost) {
    owned_resource_[resource.first] -= resource.second;
  }
}

}  // namespace agent