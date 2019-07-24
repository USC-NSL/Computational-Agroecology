#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CONFIG_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CONFIG_H_

#include <string>

#include "environment/location.h"

namespace environment {

// The configuration of this place
struct Config {
  Config(const std::string& name, const Location& location);

  const std::string name;
  const Location location;

  // TODO: define plant type set
};

bool operator==(const Config& lhs, const Config& rhs);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CONFIG_H_