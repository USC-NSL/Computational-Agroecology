#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_RESOURCE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_RESOURCE_H_

#include <unordered_map>

// Ralph: This used to be in utility.h. Because of the need of
// putting `ResourceType` in the namespace environment, I moved it here.
namespace environment {

// TODO: Complete this resource type enum.
enum class ResourceType {
  NITROGEN = 0,  // TODO
  PHOSPHORUS,
  POTASSIUM,
  CALCIUM,
  MAGNESIUM,
  SULFUR  
};

// TODO: Document this Resources map based upon the right units for the value
// type.
using Resources = std::unordered_map<ResourceType, int64_t>;

}

#endif // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_RESOURCE_H_