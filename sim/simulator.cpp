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

  DayWeather& dayWeather = state.getWeather()
      .day_weather_list()[state.getCurrentDay()];

  std::vector<std::vector<Cell>>& terrain = state.getTerrain().terrain();

  for (auto& row : terrain) {
    for (auto& cell : row) {
      if (cell.plant != nullptr) {
        cell.plant->Transition(dayWeather.getRainfall(),
            dayWeather.getMinTemp(), dayWeather.getMaxTemp());
      }
    }
  }

}
void DoWeatherCycle(State& state) {
  //TODO: Not sure if needed...
}

//TODO: Add any other functions for steps required in the main simulator loop