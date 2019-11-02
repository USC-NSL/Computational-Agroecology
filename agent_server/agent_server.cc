#include "agent_server.h"

#include <utility>

namespace agent_server {

AgentServer::ReturnCodes AgentServer::CreateEnvironment(
    const std::string &name, const config::Config &config,
    const config::TerrainRawData &terrain_raw_data,
    const std::chrono::system_clock::time_point &time,
    const std::chrono::duration<int> &time_step_length) {
  auto result = name_to_env_.find(name);
  if (result != name_to_env_.end()) {
    return ALREADY_EXISTS;
  }

  name_to_env_.emplace(
      std::make_pair(name, environment::Environment(config, terrain_raw_data,
                                                    time, time_step_length)));

  return OK;
}

AgentServer::ReturnCodes AgentServer::DeleteEnvironment(
    const std::string &name) {
  auto result = name_to_env_.find(name);
  if (result == name_to_env_.end()) {
    return ENV_NOT_FOUND;
  }

  name_to_env_.erase(result);
  return OK;
}

AgentServer::ReturnCodes AgentServer::CreateQLearningAgent(
    const std::string &agent_name, const std::string &env_name, const int row,
    const int col) {
  auto agent_result = name_to_agent_.find(agent_name);
  if (agent_result != name_to_agent_.end()) {
    return ALREADY_EXISTS;
  }

  auto env_result = name_to_env_.find(env_name);
  if (env_result == name_to_env_.end()) {
    return ENV_NOT_FOUND;
  }

  name_to_agent_.emplace(std::make_pair(
      agent_name,
      new agent::Qlearning(agent_name, &(env_result->second), row, col)));

  return OK;
}

AgentServer::ReturnCodes AgentServer::DeleteAgent(const std::string &name) {
  auto result = name_to_agent_.find(name);
  if (result == name_to_agent_.end()) {
    return AGENT_NOT_FOUND;
  }

  name_to_agent_.erase(result);
  return OK;
}

std::pair<AgentServer::ReturnCodes, std::optional<environment::Environment>>
AgentServer::GetEnvironment(const std::string &name) {
  auto result = name_to_env_.find(name);
  if (result == name_to_env_.end()) {
    return std::make_pair(ENV_NOT_FOUND, std::nullopt);
  }

  return std::make_pair(OK, result->second);
}

AgentServer::ReturnCodes AgentServer::SimulateToTimeStep(
    const std::string &env_name, const int64_t time_step) {
  auto env_it = name_to_env_.find(env_name);
  if (env_it == name_to_env_.end()) {
    return ENV_NOT_FOUND;
  }

  env_it->second.JumpToTimeStep(time_step);

  return OK;
}

AgentServer::ReturnCodes AgentServer::AgentTakeAction(
    const std::string &agent_name, const agent::action::Action *action) {
  auto agent_it = name_to_agent_.find(agent_name);
  if (agent_it == name_to_agent_.end()) {
    return AGENT_NOT_FOUND;
  }

  auto action_ret = agent_it->second->TakeAction(action);

  if (action_ret == agent::Agent::NOT_ENOUGH_RESOURCES) {
    return ACTION_NOT_ENOUGH_RESOURCES;
  } else if (action_ret == agent::Agent::SUCCESS) {
    return OK;
  }

  return UNKNOWN_ERROR;
}

}  // namespace agent_server