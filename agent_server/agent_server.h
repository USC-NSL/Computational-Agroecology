#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_H_

#include <map>
#include <optional>
#include <string>
#include <utility>

#include "environment/agent/agent.h"
#include "environment/config.h"
#include "environment/environment.h"
#include "environment/terrain.h"

namespace agent_server {

class AgentServer {
 public:
  enum ReturnCodes { OK = 0, NOT_FOUND, ALREADY_EXISTS };

  ReturnCodes CreateEnvironment(
      const std::string& name, const environment::Config& config,
      const std::chrono::system_clock::time_point& time,
      const environment::Terrain& terrain);
  ReturnCodes CreateAgent(const std::string& agent_name,
                          const std::string& env_name);
  std::pair<ReturnCodes, std::optional<environment::Environment>>
  GetEnvironment(const std::string& name);

 private:
  std::map<std::string, agent::Agent> name_to_agent_;
  std::map<std::string, environment::Environment> name_to_env_;
};

}  // namespace agent_server

#endif  // COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_H_