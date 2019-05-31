#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CONFIG_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CONFIG_H_

#include <string>

#include "location.h"

namespace environment {

struct Config {
  Config(const std::string& name, const Location& location);

  const std::string name;
  const Location location;

  // TODO: define plant type set
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_CONFIG_H_