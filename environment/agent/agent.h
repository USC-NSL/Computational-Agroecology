#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_

#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "agent/resource.h"
#include "environment.h"
#include "simulators/actions/crop.h"

namespace agent {

const std::string kCornTypeName = "Corn";

struct ActionID {
  size_t row;
  size_t col;
  ::simulator::action::ActionType action_taken;
  int crop_ID;
};

class Agent {
 public:
  enum ReturnCodes { SUCCESS = 0, INVALID_ARGUMENT, NOT_ENOUGH_RESOURCES };

  // modifiers
  void AddResource(const agent::ResourceType &resource, size_t quantity);
  ReturnCodes TakeAction(const simulator::action::Action *action);

  // generate action
  simulator::action::Action *CreateAction(const ActionID &action_id);

  // generate random integer
  int RandomInt(int min, int max);

  std::vector<std::string> GetQualifiedPlants();

  // accessors
  inline const std::string &name() const { return name_; }

  inline environment::Environment *mutable_environment() { return env_; }
  inline const environment::Environment *environment() const { return env_; }

  inline const Resources owned_resource() const { return owned_resources_; }

 protected:
  // constructors which can only be used in heritance
  // only children can call these
  Agent(const std::string &name, environment::Environment *env);
  Agent(const std::string &name, environment::Environment *env,
        const Resources &owned_resources);

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