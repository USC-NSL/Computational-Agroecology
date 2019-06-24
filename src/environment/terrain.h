#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_

#include <iostream>
#include <optional>
#include <vector>

#include "plant.h"
#include "soil.h"

namespace environment {

// A wrapper to represent the position on the `Tiles`
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
  std::optional<Plant> plant;
  Soil soil;
};

bool operator==(const Cell& lhs, const Cell& rhs);

// A collection of 2D `Cell`s
class Tiles : public std::vector<std::vector<Cell>> {
 public:
  Tiles();
  Tiles(const size_t width, const size_t length, const Soil& soil);
  inline Cell& get(const Coordinate& coordinate) {
    return (*this)[coordinate.x][coordinate.y];
  }

  inline const size_t width() const { return size(); }
  inline const size_t length() const { return empty() ? 0 : front().size(); }
};

// TODO: think about whether it is necessary to define a `class Tiles`
// The data structure that stores the data of the crop field
class Terrain {
 public:
  // Constructor
  // TODO: add more constructors to import different kinds of terrain
  // currently, this is just a dumb constructor which ignores lots of details
  Terrain(const size_t size);

  // Accessors
  inline const size_t width() const { return tiles_.width(); }
  inline const size_t length() const { return tiles_.length(); }
  inline Tiles& tiles() { return tiles_; }
  inline const int yield() { return yield_; }
  inline const Tiles& tiles() const { return tiles_; }
  inline void AddYield(int produce) { yield_ += produce; }

 private:
  friend std::ostream& operator<<(std::ostream& os, const Terrain& terrain);

  Tiles tiles_;
  int yield_;
};

std::ostream& operator<<(std::ostream& os, const Terrain& terrain);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_