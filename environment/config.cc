#include "config.h"

namespace environment {

Config::Config(const std::string& name, const Location& location)
    : name(name), location(location) {}

}  // namespace environment