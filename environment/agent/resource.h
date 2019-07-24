#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_AGENT_RESOURCE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_AGENT_RESOURCE_H_

#include <unordered_map>

namespace agent {

// This defines the types of resources that an agent/human has.
enum class ResourceType { MONEY = 0, LABOR };

using Resources = std::unordered_map<ResourceType, size_t>;

}  // namespace agent

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_AGENT_RESOURCE_H_