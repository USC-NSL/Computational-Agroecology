#include "agent_server.h"

#include <utility>

namespace agent_server {

AgentServer::ReturnCodes AgentServer::CreateEnvironment(
    const std::string& name, const environment::Config& config,
    const std::chrono::system_clock::time_point& time,
    const environment::Terrain& terrain) {
  auto result = name_to_env_.find(name);
  if (result != name_to_env_.end()) {
    return ALREADY_EXISTS;
  }

  name_to_env_.emplace(
      std::make_pair(name, environment::Environment(config, time, terrain)));

  return OK;
}

AgentServer::ReturnCodes AgentServer::CreateAgent(const std::string& agent_name,
                                                  const std::string& env_name) {
  auto agent_result = name_to_agent_.find(agent_name);
  if (agent_result != name_to_agent_.end()) {
    return ALREADY_EXISTS;
  }

  auto env_result = name_to_env_.find(env_name);
  if (env_result == name_to_env_.end()) {
    return ENV_NOT_FOUND;
  }

  name_to_agent_.emplace(std::make_pair(
      agent_name, agent::Agent(agent_name, &(env_result->second))));

  return OK;
}

std::pair<AgentServer::ReturnCodes, std::optional<environment::Environment>>
AgentServer::GetEnvironment(const std::string& name) {
  auto result = name_to_env_.find(name);
  if (result == name_to_env_.end()) {
    return std::make_pair(ENV_NOT_FOUND, std::nullopt);
  }

  return std::make_pair(OK, result->second);
}

AgentServer::ReturnCodes AgentServer::SimulateToTime(
    const std::string& env_name,
    const std::chrono::system_clock::time_point& time) {
  auto env_it = name_to_env_.find(env_name);
  if (env_it == name_to_env_.end()) {
    return ENV_NOT_FOUND;
  }

  env_it->second.JumpToTime(time);

  return OK;
}

AgentServer::ReturnCodes AgentServer::AgentTakeAction(
    const std::string& agent_name, const simulator::action::Action* action) {
  auto agent_it = name_to_agent_.find(agent_name);
  if (agent_it == name_to_agent_.end()) {
    return AGENT_NOT_FOUND;
  }

  auto action_ret = agent_it->second.TakeAction(action);

  if (action_ret == agent::Agent::NOT_ENOUGH_RESOURCES) {
    return ACTION_NOT_ENOUGH_RESOURCES;
  } else if (action_ret == agent::Agent::SUCCESS) {
    return OK;
  }

  return UNKNOWN_ERROR;
}

}  // namespace agent_server