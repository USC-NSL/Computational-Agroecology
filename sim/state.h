// Copyright 2019

#ifndef COMPUTATIONAL_AGROECOLOGY_STATE_H
#define COMPUTATIONAL_AGROECOLOGY_STATE_H

#include <memory>

#include "config.h"
#include "terrain.h"

class State {
 public:
  State() = delete;
  State(Config *config, unsigned int day, Weather weather);
  State(const State& other);
  State& operator=(const State& other);

  void IncrementDay();

  Terrain& getTerrain();
  Weather& getWeather();
 private:
  std::shared_ptr<Config> conf_;
  Terrain terrain_;
  std::unique_ptr<Weather> weather_;
  unsigned int day_;
 public:
  unsigned int getCurrentDay() const;
};

#endif //COMPUTATIONAL_AGROECOLOGY_STATE_H
