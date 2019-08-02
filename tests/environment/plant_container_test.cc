#include <gtest/gtest.h>
#include <string>

#include "environment/plant_builder.h"
#include "environment/plant_container.h"

using namespace environment;

const std::string kBeanTypeName = "bean";

TEST(PlantContainerTest, AddPlantTest) {
  environment::PlantContainer plant_container;
  std::unique_ptr<environment::Plant> plant1(
      environment::PlantBuilder::NewPlant(kBeanTypeName));
  plant1->SetTrunkSize(1.0);
  std::unique_ptr<environment::Plant> plant2(
      environment::PlantBuilder::NewPlant(kBeanTypeName));
  plant2->SetTrunkSize(1.0);
  EXPECT_TRUE(plant_container.AddPlant(&(*plant1)));
  EXPECT_FALSE(plant_container.AddPlant(&(*plant2)));
}

TEST(PlantContainerTest, DeletePlantTest) {
  environment::PlantContainer plant_container;
  std::unique_ptr<environment::Plant> plant1(
      environment::PlantBuilder::NewPlant(kBeanTypeName));
  plant1->SetTrunkSize(1.0);
  plant1->SetPosition({5.0, 5.0});
  std::unique_ptr<environment::Plant> plant2(
      environment::PlantBuilder::NewPlant(kBeanTypeName));
  plant2->SetTrunkSize(1.0);
  plant2->SetPosition({0.0, 0.0});
  plant_container.AddPlant(&(*plant1));
  plant_container.AddPlant(&(*plant2));
  point_t position = plant1->position();
  EXPECT_FALSE(plant_container.DelPlant({1.0, 1.0}));
  EXPECT_TRUE(plant_container.DelPlant(position));
  EXPECT_TRUE(plant_container.DelPlant(*plant2));
  EXPECT_FALSE(plant_container.DelPlant(position));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}