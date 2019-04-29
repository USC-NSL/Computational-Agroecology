// Copyright 2019

#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "../common/plant_index.h"
#include "../common/soil.h"
#include "../common/plant_type.h"

// square cell that made up the smallest component in the terrain
typedef struct {
    bool occupied;
    int size; // in meter
    PlantType *plant;
    Soil *soil;
} cellValue;

class Terrain {
 public:
  Terrain(int width, int length)
    :width_(width), length_(length){
      tiles_.resize(width, std::vector<cellValue>(length, {false, 1, NULL, new Soil()}));
  }
  std::vector<std::vector<cellValue>> tiles() { return tiles_; }
  int width() { return width_; }
  int length() { return length_; }
  void terrain_display();
  std::vector<std::vector<cellValue>> tiles_; //tiles specify all the cells in the terrain
 private:
  int width_;
  int length_;
};




#endif
