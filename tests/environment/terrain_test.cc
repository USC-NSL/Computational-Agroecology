#include <gtest/gtest.h>

#include "environment/terrain.h"

using namespace config;
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
  Config dumb_config("place name", Location(100, 101, 201, 200));
  Climate dumb_climate(dumb_config);
  Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  Meteorology dumb_meteorology(std::chrono::system_clock::now(),
                               dumb_config.location, dumb_climate.climate_zone,
                               dumb_weather);
  TerrainRawData dumb_terrain_raw_data(5, 0);
  Terrain terrain(dumb_terrain_raw_data, dumb_meteorology);

  EXPECT_EQ(5, terrain.size());
  EXPECT_EQ(5, terrain.size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
