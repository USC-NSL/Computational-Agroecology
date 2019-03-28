//
// Created by Adam Egyed on 2019-03-27.
//

#include "state.h"

State::State(Config* c, unsigned int day) : conf(c), day(day) {
  while (day > 365) day %= 365;
  if (day < 1) this->day = 1;
}

State::State(const State &other) = default;

State &State::operator=(const State &other) = default;

void State::incrementDay() {
  day++;
  if (day > 365) day = 1;
}

