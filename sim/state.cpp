#include <memory>

//
// Created by Adam Egyed on 2019-03-27.
//

#include "state.h"

/**
 * Constructor for a brand new state.
 * @param config : The configuration for this state and all derived states to
 * use. Should be new'd into the constructor, as it will automatically be
 * deleted by State when it itself is deleted
 * @param day - The day of the year to start the simulation
 * @param weather - The weather configuration for this simulation state
 */
State::State(Config *config, unsigned int day, Weather weather)
    : conf_(config), day_(day), weather_(new Weather(weather)) {
  while (day > 365) day %= 365;
  if (day < 1) this->day_ = 1;
}

State::State(const State &other) : conf_(other.conf_),
                                   terrain_(other.terrain_), day_(other.day_),
                                   weather_(new Weather(*other.weather_)) {}


void State::IncrementDay() {
  day_++;
  if (day_ > 365) day_ = 1;
  // Ignoring leap years for now...
}

State &State::operator=(const State &other) {

  conf_ = other.conf_;
  terrain_ = other.terrain_;
  day_ = other.day_;
  weather_ = std::make_unique<Weather>(*other.weather_);

  return (*this);
}


