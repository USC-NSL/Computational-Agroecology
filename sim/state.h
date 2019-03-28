//
// Created by Adam Egyed on 2019-03-27.
//

#ifndef COMPUTATIONAL_AGROECOLOGY_STATE_H
#define COMPUTATIONAL_AGROECOLOGY_STATE_H

#include <memory>

#include "config.h"
#include "terrain.h"

class State {
 public:
  State() = delete;
  State(Config* config, unsigned int day);
  State(const State& other);
  State& operator=(const State& other);

  void incrementDay();
 private:
  std::shared_ptr<Config> conf;
  Terrain terrain;
  unsigned int day;
};

#endif //COMPUTATIONAL_AGROECOLOGY_STATE_H
