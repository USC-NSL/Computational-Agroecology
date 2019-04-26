// Copyright 2019

#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "../common/plant.h"
#include "../common/soil.h"

// square cell that made up the smallest component in the terrain
struct cellValue {
    bool occupied;
    int size; // in meter
    Plant *plant;
    Soil *soil;
};

class Terrain {
 public:
  Terrain(int width, int length)
    :width_(width), length_(length){
      tiles_.resize(width, std::vector<cellValue>(length, {false, 1, NULL, NULL}));
  }
  std::vector<std::vector<cellValue>> tiles() { return tiles_; }
  int width() { return width_; }
  int length() { return length_; }

 private:
  std::vector<std::vector<cellValue>> tiles_; //tiles specify all the cells in the terrain
  int width_;
  int length_;
};




#endif
