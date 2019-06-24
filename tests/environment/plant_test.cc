#include <string>

#include <gtest/gtest.h>

#include "environment/plant.h"

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

TEST(PlantTest, OperatorTest) {
  const std::string kCornTypeName = "Corn";
  Plant lhs(kCornTypeName);
  Plant rhs(kCornTypeName);

  EXPECT_TRUE(lhs == rhs);

  rhs.accumulated_gdd = 1;
  EXPECT_FALSE(lhs == rhs);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
