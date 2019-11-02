#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_H_

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "agent/actions/crop.h"
#include "agent/q_learning.h"
#include "config/config.h"
#include "environment/environment.h"
#include "environment/terrain.h"

namespace agent_server {

class AgentServer {
 public:
  enum ReturnCodes {
    OK = 0,
    AGENT_NOT_FOUND,
    ENV_NOT_FOUND,
    ALREADY_EXISTS,
    ACTION_NOT_ENOUGH_RESOURCES,
    UNKNOWN_ERROR
  };

  ReturnCodes CreateEnvironment(
      const std::string &name, const config::Config &config,
      const config::TerrainRawData &terrain_raw_data,
      const std::chrono::system_clock::time_point &time,
      const std::chrono::duration<int> &time_step_length);
  ReturnCodes DeleteEnvironment(const std::string &name);

  ReturnCodes CreateQLearningAgent(const std::string &agent_name,
                                   const std::string &env_name, const int row,
                                   const int col);

  ReturnCodes DeleteAgent(const std::string &name);

  std::pair<ReturnCodes, std::optional<environment::Environment>>
  GetEnvironment(const std::string &name);
  ReturnCodes SimulateToTimeStep(const std::string &env_name,
                                 const int64_t time_step);
  ReturnCodes AgentTakeAction(const std::string &agent_name,
                              const agent::action::Action *action);

 private:
  std::map<std::string, std::shared_ptr<agent::Agent>> name_to_agent_;
  std::map<std::string, environment::Environment> name_to_env_;
};

}  // namespace agent_server

#endif  // COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_H_