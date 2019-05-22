// copyright 2019
#include "config.h"

Config::Config(Location loc, std::vector<PlantType> plants)
    : location_(loc), plants_(std::move(plants)) {
  Weather weather(15, 26, 1); // set up temporary weather component
  terrain_ = new Terrain(10, 10);
  for (int i = 0; i < 365; i++) {
    yearly_weather_.push_back(weather);
  }
}

int Config::perform_daily_actions() {
  int success_action = 0;
  while (!daily_actions_.empty()) {
    success_action++;
    ActionAdapter *action = daily_actions_.back();
    daily_actions_.pop_back();
    // (*daily_actions.back()).perform_action(config.terrain_, config.plants());
    (*action).perform_action(terrain_, plants_);
  }
  return success_action;
}

void Config::state_display(int current_day) {
  printf("==================Day %d==================\n", current_day);
  yearly_weather_[current_day].weather_display();
  terrain_->terrain_display();
}

PlantType *Config::get_plant_status(int date, int x, int y) {
  return terrain_->get_plant_status(date, x, y);
}

// used to calculate Yield
double Config::calculateYield()
{
  double weight = 0;
  for(int i = 0; i < this->plants_.size(); i++)
  {
    weight+=(plants_[i].getProduceWeight()); // add each plant's produceWeight
  }
  return weight/((terrain_->width())*(terrain_->length()));
}