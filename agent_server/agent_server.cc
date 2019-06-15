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
    return NOT_FOUND;
  }

  name_to_agent_.emplace(std::make_pair(
      agent_name, agent::Agent(agent_name, &(env_result->second))));

  return OK;
}

std::pair<AgentServer::ReturnCodes, std::optional<environment::Environment>>
AgentServer::GetEnvironment(const std::string& name) {
  auto result = name_to_env_.find(name);
  if (result == name_to_env_.end()) {
    return std::make_pair(NOT_FOUND, std::nullopt);
  }

  return std::make_pair(OK, result->second);
}

}  // namespace agent_server