//
// Created by Adam Egyed on 2019-03-27.
//

#include <vector>
#include "state.h"

void UpdatePlantGrowth(State& state);
void SoilImpact(State& state);

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

  DayWeather& dayWeather = state.getWeather()
      .day_weather_list()[state.getCurrentDay()];

  std::vector<std::vector<Cell>>& terrain = state.getTerrain().terrain();

  for (auto& row : terrain) {
    for (auto& cell : row) {
      if (cell.plant != nullptr) {
        cell.plant->Transition(dayWeather.getRainfall(),
            dayWeather.getMinTemp(), dayWeather.getMaxTemp());
        SoilImpact(state);
      }
    }
  }

}
void SoilImpact(State& state) {
  //TODO: Fill in as we work out the model
}

//TODO: Add any other functions for steps required in the main simulator loop
// with a similar function signature: passing in a state reference and
// calling it in the loop.
// These could include light tracing, water/runoff sims, and soil composition