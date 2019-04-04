//
// Created by Adam Egyed on 2019-03-27.
//

#include "state.h"

State::State(Config* c, unsigned int day) : conf_(c), day_(day) {
  while (day > 365) day %= 365;
  if (day < 1) this->day_ = 1;
}

State::State(const State &other) : conf_(other.conf_),
terrain_(other.terrain_), day_(other.day_) {
  weather_ = new Weather(other.weather_);
}


void State::IncrementDay() {
  day_++;
  if (day_ > 365) day_ = 1;
}
State &State::operator=(const State &other) {

  conf_ = other.conf_;
  terrain_ = other.terrain_;
  day_ = other.day_;
  weather_ = new Weather(other.weather_);

  return (*this);
}


