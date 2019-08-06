#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "agent/actions/action.h"
#include "environment/plant.h"
#include "environment/soil.h"

namespace environment {

// A wrapper to represent the position on the `Tiles`
struct Coordinate {
  Coordinate(const double x, const double y, const double z = 0);
  Coordinate(std::vector<double> position);

  double x;
  double y;
  double z;
};

bool operator==(const Coordinate &lhs, const Coordinate &rhs);

struct Cell {
  Cell(const Soil &soil);
  Cell(const size_t size, const Soil &soil);

  size_t size;
  std::shared_ptr<Plant> plant;
  Soil soil;
};

bool operator==(const Cell &lhs, const Cell &rhs);

// A collection of 2D `Cell`s
class Tiles : public std::vector<std::vector<Cell>> {
 public:
  Tiles();
  Tiles(const size_t width, const size_t length, const Soil &soil);
  inline Cell &get(const Coordinate &coordinate) {
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
  inline const Tiles &tiles() const { return tiles_; }
  inline const int yield() const { return yield_; }

  // Modifiers
  void ExecuteAction(const agent::action::Action &action);

 private:
  friend std::ostream &operator<<(std::ostream &os, const Terrain &terrain);

  // befriend with a list of actions
  friend class agent::action::crop::Add;
  friend class agent::action::crop::Remove;
  friend class agent::action::crop::Harvest;
  friend class agent::action::crop::Water;

  Tiles tiles_;
  int yield_;
};

std::ostream &operator<<(std::ostream &os, const Terrain &terrain);

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_TERRAIN_H_
