#include "terrain.h"

#include <vector>

#include "plant_types/plant_type.h"

namespace environment {

// `struct Coordinate`
Coordinate::Coordinate(const size_t x, const size_t y) : x(x), y(y) {}

bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

// `struct Cell`
Cell::Cell(const Soil& soil) : Cell(1, soil) {}

Cell::Cell(const size_t size, const Soil& soil)
    : size(size), plant(nullptr), soil(soil) {}

bool operator==(const Cell& lhs, const Cell& rhs) {
  bool ret = true;
  ret &= (lhs.size == rhs.size);
  ret &= (lhs.plant == rhs.plant);
  ret &= (lhs.soil == rhs.soil);
  return ret;
}

// `class Tiles`
Tiles::Tiles() : std::vector<std::vector<Cell>>() {}
Tiles::Tiles(const size_t width, const size_t length, const Soil& soil)
    : std::vector<std::vector<Cell>>(width, std::vector<Cell>(length, soil)) {}

// `class Terrain`
Terrain::Terrain(const size_t size) : tiles_() {
  Soil dumb_soil(Soil::CLAY, 7.0, 0.0, 0.0, 0.0);
  tiles_ = Tiles(size, size, dumb_soil);
}

std::ostream& operator<<(std::ostream& os, const Terrain& terrain) {
  for (const auto& row : terrain.tiles_) {
    for (const auto& cell : row) {
      if (cell.plant != nullptr) {
        auto& plant_type =
            plant_type::plant_type_to_plant[cell.plant->type_name];
        os << plant_type->display_symbol;
      } else {
        os << " ";
      }
    }
    os << '\n';
  }

  return os;
}

}  // namespace environment