// Copyright 2019
#include "weather.h"

void Weather::weather_display() {
  printf("Max temperature for today: %lf\n", temp_max_);
  printf("Min temperature for today: %lf\n", temp_min_);
  printf("rainfall for today: %lf\n", rainfall_);
}