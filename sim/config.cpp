//
// Created by Adam Egyed on 2019-03-27.
//

#include "config.h"


Config::~Config() = default;

Config* Config::buildConfig(double latitude, double longitude,
    Weather weather) {
  return new Config(latitude, longitude, weather);
}

Config::Config(double lat, double lon, Weather w) : latitude(lat),
    longitude(lon), weather(w) {}

Config::Config(const Config &other) = default;

Config &Config::operator=(const Config &other) = default;
