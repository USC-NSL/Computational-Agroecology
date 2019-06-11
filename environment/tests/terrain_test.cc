#include <gtest/gtest.h>

#include "terrain.h"

using namespace environment;

TEST(CoordinateTest, ConstructorTest) {
  Coordinate c(0, 1);
  EXPECT_EQ(0, c.x);
  EXPECT_EQ(1, c.y);
}

TEST(CoordinateTest, OperatorTest) {
  Coordinate c_1(0, 1);
  Coordinate c_2(0, 1);
  EXPECT_TRUE(c_1 == c_2);

  c_2.y = 2;
  EXPECT_FALSE(c_1 == c_2);
}

TEST(CellTest, ConstructorTest_1) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);

  Cell cell(soil);
  EXPECT_EQ(1, cell.size);
  EXPECT_EQ(nullptr, cell.plant);
  EXPECT_EQ(soil, cell.soil);
}

TEST(CellTest, ConstructorTest_2) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);

  Cell cell(2, soil);
  EXPECT_EQ(2, cell.size);
  EXPECT_EQ(nullptr, cell.plant);
  EXPECT_EQ(soil, cell.soil);
}

TEST(CellTest, OperatorTest) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);

  Cell lhs(2, soil);
  Cell rhs(2, soil);

  EXPECT_TRUE(lhs == rhs);

  rhs.size = 3;
  EXPECT_FALSE(lhs == rhs);
}

TEST(TilesTest, ConstructorTest_1) {
  Tiles tiles;

  EXPECT_TRUE(tiles.empty());
  EXPECT_EQ(0, tiles.width());
  EXPECT_EQ(0, tiles.length());
}

TEST(TilesTest, ConstructorTest_2) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);
  Tiles tiles(5, 7, soil);

  EXPECT_EQ(5, tiles.width());
  EXPECT_EQ(7, tiles.length());

  Cell a_default_cell(soil);
  for (const auto& row : tiles) {
    for (const Cell& c : row) {
      EXPECT_EQ(a_default_cell, c);
    }
  }
}

TEST(TilesTest, GetTest) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);
  Tiles tiles(5, 7, soil);

  EXPECT_EQ(tiles[1][1], tiles.get(Coordinate(1, 1)));

  tiles.get(Coordinate(2, 2)).size = 3;
  EXPECT_EQ(tiles[2][2], tiles.get(Coordinate(2, 2)));

  tiles[3][3].size = 4;
  EXPECT_EQ(tiles[3][3], tiles.get(Coordinate(3, 3)));
}

TEST(TerrainTest, ConstructorTest) {
  Terrain terrain(5);

  EXPECT_EQ(5, terrain.width());
  EXPECT_EQ(5, terrain.length());
  EXPECT_EQ(5, terrain.tiles().width());
  EXPECT_EQ(5, terrain.tiles().length());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}