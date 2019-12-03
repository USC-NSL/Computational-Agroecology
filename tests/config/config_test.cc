#include <string>

#include <gtest/gtest.h>

#include "config/config.h"

using namespace config;

TEST(ConfigTest, ConstructorTest) {
  std::string place_name("place name");
  Location location(100, 100, 200, 200);

  Config config(place_name, location);

  EXPECT_EQ(config.name, place_name);
  EXPECT_EQ(config.location, location);
}

TEST(ConfigTest, OperatorTest) {
  Config lhs("place name", Location(100, 100, 200, 200));
  Config rhs("place name", Location(100, 100, 200, 200));

  EXPECT_TRUE(lhs == rhs);

  Config new_rhs("place name", Location(100, 100, 200, 500));
  EXPECT_FALSE(lhs == new_rhs);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
