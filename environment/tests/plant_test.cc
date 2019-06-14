#include <string>

#include <gtest/gtest.h>

#include "plant.h"

using namespace environment;

TEST(PlantTest, ConstructorTest) {
  const std::string kCornTypeName = "Corn";
  Plant plant(kCornTypeName);
  EXPECT_EQ(kCornTypeName, plant.type_name);
  EXPECT_EQ(kInitialHealth, plant.health);
  EXPECT_EQ(false, plant.flowering);
  EXPECT_EQ(0, plant.accumulated_gdd);
  EXPECT_EQ(Plant::SEED, plant.maturity);
  EXPECT_EQ(0.0, plant.base_temperature);
  EXPECT_TRUE(plant.gdd_thresholds.empty());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
