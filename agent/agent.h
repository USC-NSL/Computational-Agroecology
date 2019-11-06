#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_

#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "agent/actions/crop.h"
#include "agent/resource.h"
#include "environment/environment.h"

namespace agent {

const std::string kBeanTypeName = "bean";

struct ActionID {
  int row;
  int col;
  ::agent::action::ActionType action_taken;
  // TODO : Set starttime = 1 duration  = 0 for now
  int start_time_step = 1;
  int duration = 0;
  const std::string crop_type_name;
};

class Agent {
 public:
  Agent(const std::string &name, environment::Environment *env);
  Agent(const std::string &name, environment::Environment *env,
        const Resources &owned_resources);

  enum ReturnCodes { SUCCESS = 0, INVALID_ARGUMENT, NOT_ENOUGH_RESOURCES };

  // modifiers
  void AddResource(const agent::ResourceType &resource, size_t quantity);
  ReturnCodes TakeAction(const agent::action::Action *action);

  // generate action
  agent::action::Action *CreateAction(const ActionID &action_id);

  // generate random integer
  int RandomInt(int min, int max);

  // Applied random action to the encironment
  // TODO: creeate random plant
  void ApplyRandomAction(int action_tyoe, int timestep);

  std::vector<std::string> GetQualifiedPlants();

  // accessors
  inline const std::string &name() const { return name_; }

  inline environment::Environment *mutable_environment() { return env_; }
  inline const environment::Environment *environment() const { return env_; }

  inline const Resources owned_resource() const { return owned_resources_; }

 protected:
  // Name of this agent
  std::string name_;

  // The corresponding environment that this agent is interacting with
  environment::Environment *env_;

  // The resources this agent has
  Resources owned_resources_;

  // Check whether this agent's resources is more than the specified resources
  bool CheckEnoughResources(const Resources &resources) const;

  // Decrease this agent's recources by the specified `cost`. This function
  // assumes this agent's resources are enough to be decreased, which means
  // `CheckEnoughResources()` should always be called before calling this
  // function.
  void DeductResources(const Resources &cost);
};

}  // namespace agent

#endif  // COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_