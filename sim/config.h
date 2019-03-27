//
// Created by Adam Egyed on 2019-03-27.
//

#ifndef COMPUTATIONAL_AGROECOLOGY_CONFIG_H
#define COMPUTATIONAL_AGROECOLOGY_CONFIG_H

#include "weather.h"

class Config {
 public:
  Config() = delete; // Do not use, never want uninitialized config
  ~Config();
  Config(const Config& other);
  Config& operator=(const Config& other);

  static Config* buildConfig(double latitude, double longitude, Weather weather, );

 private:
  Config(double lat, double lon, Weather w);

  // Fields
  double latitude;
  double longitude;
  Weather weather;
};

#endif //COMPUTATIONAL_AGROECOLOGY_CONFIG_H
