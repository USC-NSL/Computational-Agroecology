//
// Created by Adam Egyed on 2019-03-27.
//

#include "state.h"

void UpdatePlantGrowth(State& state);
void DoWeatherCycle(State& state);

State SimulateTimeStep(const State& state, int days) {
  State newState = state;

  for (int i = 0; i < days; ++i) {
    UpdatePlantGrowth(newState);
    DoWeatherCycle(newState);
    // Any other necessary steps in advancing 1 day

    newState.IncrementDay();
  }

  return newState;
}

void UpdatePlantGrowth(State& state) {
  //TODO: Fill with whatever is necessary
}
void DoWeatherCycle(State& state) {
  //TODO: Fill with whatever is necessary
}

//TODO: Add any other functions for steps required in the main simulator loop