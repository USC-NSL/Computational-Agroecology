// Copyright 2019
#ifndef COMPUTATIONAL_AGROECOLOGY_CONFIG_H
#define COMPUTATIONAL_AGROECOLOGY_CONFIG_H

#include "../sim/terrain.cpp"
#include "../sim/terrain.h"
#include "action_adapter.h"
#include "climate.h"
#include "location.h"
#include "plant_type.h"
#include "weather.cpp"
#include "weather.h"
#include <stdio.h>
#include <vector>

class Config {
public:
  Config(Location loc, std::vector<PlantType> plants);
  void state_display(int current_day);
  PlantType *get_plant_status(int date, int x, int y);
  Location location() { return location_; }
  std::vector<PlantType> plants() { return plants_; }
  Terrain terrain() { return *terrain_; }
  std::vector<Weather> yearly_weather() { return yearly_weather_; }
  std::vector<ActionAdapter *> daily_actions() { return daily_actions_; }
  bool add_daily_action(ActionAdapter *actionAdapter) {
    daily_actions_.push_back(actionAdapter);
  }
  int perform_daily_actions();

  Terrain *terrain_; // current terrain
  std::vector<ActionAdapter *>
      daily_actions_; // all actions that's going to perform in a day
private:
  Location location_;             // latitude and longitude of location
  std::vector<PlantType> plants_; // all available plants in the field
  Climate climate_;               // climate of the location
  std::vector<Weather>
      yearly_weather_; // weather simulating for location all year round
};

#endif // COMPUTATIONAL_AGROECOLOGY_CONFIG_H