#ifndef TERRAIN_H
#define TERRAIN_H

#include <common/cell.h>

class Terrain {
 public:
  Terrain();
  ~Terrain();

 private:
  Cell terrain[10000][10000];
};

#endif
