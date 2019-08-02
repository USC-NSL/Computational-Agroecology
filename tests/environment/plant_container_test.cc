#include <gtest/gtest.h>
#include <string>

#include "environment/plant_builder.h"
#include "environment/plant_container.h"

using namespace environment;

const std::string kBeanTypeName = "bean";

TEST(PlantContainerTest, PlantSuccessTest) {
  environment::PlantContainer plant_container;
  std::unique_ptr<environment::Plant> plant(
      environment::PlantBuilder::NewPlant(kBeanTypeName));
  bool result = plant_container.AddPlant(&(*plant));
  EXPECT_TRUE(result == true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}