//
// Created by Adam Egyed on 2019-03-27.
//

#include <vector>
#include "state.h"

void UpdatePlantGrowth(State& state);
void DoWeatherCycle(State& state);

State SimulateTimeStep(const State& state, int days) {
  State newState = state;

  for (int i = 0; i < days; ++i) {
    UpdatePlantGrowth(newState);
    // Any other necessary steps in advancing 1 day

    newState.IncrementDay();
  }

  return newState;
}

void UpdatePlantGrowth(State& state) {
  //TODO: get accurate weather readings, currently weather only supports yearly
  int rainfall = 0;
  int minTemp = 0;
  int maxTemp = 0;

  std::vector<std::vector<Cell>>& terrain = state.getTerrain().terrain();

  for (int i = 0; i < terrain.size(); ++i) {
    for (int j = 0; j < terrain[i].size(); ++j) {
      if (terrain[i][j].plant_ != nullptr) {
        terrain[i][j].plant_->Transition(rainfall, minTemp, maxTemp);
      }
    }
  }

}
void DoWeatherCycle(State& state) {
  //TODO: Not sure if needed...
}

//TODO: Add any other functions for steps required in the main simulator loop