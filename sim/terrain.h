#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "../common/cell.h"

class Terrain {
 public:
  explicit Terrain(unsigned long size = 10000);
  ~Terrain();

 private:
  std::vector< std::vector<Cell>> terrain_;
};


#endif
