#include <string>

#include <gtest/gtest.h>

#include "environment/plant_builder.h"
#include "environment/plant_container.h"

using namespace environment;

const std::string kBeanTypeName = "bean";

TEST(PlantContainerTest, AddPlantTest) {
  PlantContainer plant_container;
  Coordinate pos(0, 0, 0);
  Plant *plant1 = plant_container.AddPlant(kBeanTypeName, pos);
  ASSERT_NE(nullptr, plant1);
  EXPECT_EQ(pos, plant1->position());
  plant1->set_trunk_size(1.0);

  Plant *plant2 = plant_container.AddPlant(kBeanTypeName, pos);
  ASSERT_EQ(nullptr, plant2);

  Plant *find = plant_container.FindPlant(pos);
  EXPECT_EQ(plant1, find);
}

TEST(PlantContainerTest, DeletePlantTest) {
  environment::PlantContainer plant_container;
  Plant *plant1 = plant_container.AddPlant(kBeanTypeName, Coordinate(5.0, 5.0));
  ASSERT_NE(nullptr, plant1);
  plant1->set_trunk_size(1.0);

  Plant *plant2 = plant_container.AddPlant(kBeanTypeName, Coordinate(0.0, 0.0));
  ASSERT_NE(nullptr, plant2);
  plant2->set_trunk_size(1.0);

  auto position = plant1->position();
  EXPECT_FALSE(plant_container.DelPlant({1.0, 1.0}));
  EXPECT_TRUE(plant_container.DelPlant(position));
  EXPECT_EQ(nullptr, plant_container.FindPlant(position));
  EXPECT_FALSE(plant_container.DelPlant(position));

  position = plant2->position();
  EXPECT_TRUE(plant_container.DelPlant(*plant2));
  EXPECT_EQ(nullptr, plant_container.FindPlant(position));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

