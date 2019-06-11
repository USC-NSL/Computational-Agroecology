#include <string>

#include <gtest/gtest.h>

#include "plant.h"

using namespace environment;

TEST(PlantTest, ConstructorTest) {
  const std::string kCornTypeName = "Corn";
  Plant plant(kCornTypeName);
  EXPECT_EQ(kCornTypeName, plant.type_name());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
