#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_

#include <iostream>
#include <vector>

#include "plant.h"
#include "soil.h"

namespace environment {

struct Coordinate {
  Coordinate(const size_t x, const size_t y);

  size_t x;
  size_t y;
};

bool operator==(const Coordinate& lhs, const Coordinate& rhs);

struct Cell {
  Cell(const Soil& soil);
  Cell(const size_t size, const Soil& soil);

  size_t size;
  Plant* plant;
  Soil soil;
};

bool operator==(const Cell& lhs, const Cell& rhs);

class Tiles : public std::vector<std::vector<Cell>> {
 public:
  Tiles();
  Tiles(const size_t width, const size_t length, const Soil& soil);
  inline Cell& get(const Coordinate& coordinate) {
    return (*this)[coordinate.x][coordinate.y];
  }

  inline const size_t width() const { return this->size(); }
  inline const size_t length() const {
    return this->empty() ? 0 : this->front().size();
  }
};

// TODO: think about whether it is necessary to define a `class Tiles`
class Terrain {
 public:
  // TODO: add more constructors to import different kinds of terrain
  // currently, this is just a dumb constructor which ignores lots of details
  Terrain(const size_t size);

  // TODO: define it
  const int score();

  // accessors
  inline const size_t width() const { return tiles_.width(); }
  inline const size_t length() const { return tiles_.length(); }
  inline Tiles& tiles() { return tiles_; }
  inline const Tiles& tiles() const { return tiles_; }

 private:
  Tiles tiles_;

  friend std::ostream& operator<<(std::ostream& os, const Terrain& terrain);
};

std::ostream& operator<<(std::ostream& os, const Terrain& terrain);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_