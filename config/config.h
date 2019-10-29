#ifndef COMPUTATIONAL_AGROECOLOGY_CONFIG_CONFIG_H_
#define COMPUTATIONAL_AGROECOLOGY_CONFIG_CONFIG_H_

#include <string>

#include "config/location.h"

namespace config {

// The configuration of this place
struct Config {
  Config(const std::string &name, const Location &location);

  const std::string name;
  const Location location;

  // TODO: define plant type set
};

bool operator==(const Config &lhs, const Config &rhs);

}  // namespace config

#endif  // COMPUTATIONAL_AGROECOLOGY_CONFIG_CONFIG_H_