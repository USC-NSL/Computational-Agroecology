#include <gtest/gtest.h>

#include "environment/terrain.h"

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

TEST(TerrainTest, ConstructorTest) {
  Terrain terrain(5);

  EXPECT_EQ(5, terrain.size());
  EXPECT_EQ(5, terrain.size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
