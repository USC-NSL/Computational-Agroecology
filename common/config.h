// Copyright 2019
#ifndef COMPUTATIONAL_AGROECOLOGY_CONFIG_H
#define COMPUTATIONAL_AGROECOLOGY_CONFIG_H

#include <vector>
#include "climate.h"
#include "location.h"
#include "plant_type.h"
#include "../sim/terrain.h"
#include "weather.h"
#include "action_adapter.h"

class Config {
 public:
  Config(Location loc, std::vector<PlantType> plants)
      : location_(loc), plants_(std::move(plants)) {
      Weather weather(15, 26, 1);
      terrain_ = new Terrain(100, 100);
      for(int i = 0; i < 365; i++)
      {
          yearly_weather_.push_back(weather);
      }
  }

  Location location() { return location_; }
  std::vector<PlantType> plants() { return plants_; }
  Terrain terrain() { return *terrain_; }
  std::vector<Weather> yearly_weather() { return yearly_weather_; }
  std::vector<ActionAdapter> daily_actions() { return daily_actions_; }
  bool add_daily_action(ActionAdapter actionAdapter) { daily_actions_.push_back(actionAdapter); }
  int perform_daily_actions() {
      int success_action = 0;
      while(!daily_actions_.empty()) {
          success_action ++;
          ActionAdapter action = daily_actions_.back();
          daily_actions_.pop_back();
          action.perform_action(*terrain_, plants_);
      }
      return success_action;
  }

 private:
  Location location_;              // latitude and longitude of location
  std::vector<PlantType> plants_;  // all available plants in the field
  Terrain *terrain_;  // current terrain
  Climate climate_; // climate of the location
  std::vector<Weather> yearly_weather_; // weather simulating for location all year round
  std::vector<ActionAdapter> daily_actions_; // all actions that's going to perform in a day
};

#endif //COMPUTATIONAL_AGROECOLOGY_CONFIG_H