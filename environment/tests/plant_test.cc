

#include <gtest/gtest.h>

#include "plant.h"

using namespace environment;

TEST(PlantTest, ConstructorTest) {
  Plant plant(plant_type::CORN);
  EXPECT_EQ(plant_type::CORN, plant.type);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
