#include <string>

#include <gtest/gtest.h>

#include "environment/plant_builder.h"

using namespace environment;

const std::string kBeanTypeName = "bean";

TEST(BeanTest, GenerateTest) {
  std::unique_ptr<environment::Plant> plant(
      environment::PlantBuilder::NewPlant(kBeanTypeName));
  EXPECT_EQ(kBeanTypeName, plant->name());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}