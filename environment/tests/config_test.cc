#include <string>

#include <gtest/gtest.h>

#include "config.h"

using namespace environment;

TEST(ConfigTest, ConstructorTest) {
  std::string place_name("place name");
  Location location(100, 100, 200, 200);

  Config config(place_name, location);

  EXPECT_EQ(config.name, place_name);
  EXPECT_EQ(config.location, location);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
