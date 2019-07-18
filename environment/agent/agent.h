#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_

#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "environment.h"
#include "simulators/actions/action.h"
#include "simulators/actions/crop.h"
#include "simulators/resource.h"

namespace agent {

const std::string kCornTypeName = "Corn";

using ResourceList = std::vector<std::pair<simulator::ResourceType, size_t>>;

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
  void AddResource(const simulator::ResourceType &resource, size_t quantity);
  ReturnCodes TakeAction(const simulator::action::Action *action);

  // generate action
  simulator::action::Action *CreateAction(const ActionID &action_id);

  // generate random integer
  int RandomInt(int min, int max);

  std::vector<std::string> GetQualifiedPlants();
  std::vector<std::string> GetOptimalPlants();

  // accessors
  inline const std::string &name() const { return name_; }

  inline environment::Environment *environment() { return env_; }
  inline const environment::Environment *environment() const { return env_; }

  inline const std::unordered_map<simulator::ResourceType, size_t>
  owned_resource() const {
    return owned_resource_;
  }

protected:
  // constructors which can only be used in heritance
  // only children can call these
  Agent(const std::string &name, environment::Environment *env);
  Agent(const std::string &name, environment::Environment *env,
        const std::unordered_map<simulator::ResourceType, size_t>
            &owned_resource);
  Agent(const std::string &name, environment::Environment *env,
        const ResourceList &resources_list);

  // Name of this agent
  std::string name_;

  // The corresponding environment that this agent is interacting with
  environment::Environment *env_;

  std::unordered_map<simulator::ResourceType, size_t> owned_resource_;

  bool CheckEnoughResources(const ResourceList &resources) const;

  void DeductResources(const ResourceList &cost);
};

} // namespace agent

#endif // COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_