#include <memory>
#include <string>

#include <gtest/gtest.h>

#include "environment/plant_builder.h"
#include "environment/plant_container.h"

using namespace config;
using namespace environment;

const std::string kBeanTypeName = "bean";

class PlantContainerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    Location dumb_location(100, 101, 201, 200);
    Config dumb_config("place name", dumb_location);
    Climate dumb_climate(dumb_config);
    Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    meteorology_.reset(
        new Meteorology(std::chrono::system_clock::now(), dumb_config.location,
                        dumb_climate.climate_zone, dumb_weather));
  }

  std::unique_ptr<Meteorology> meteorology_;
  PlantContainer plant_container_;
};

TEST_F(PlantContainerTest, AddPlantTest) {
  Coordinate pos(0, 0, 0);
  std::unique_ptr<Plant> plant1(
      plant_container_.AddPlant(kBeanTypeName, pos, *meteorology_));
  ASSERT_NE(nullptr, plant1);
  EXPECT_EQ(pos, plant1->position());
  plant1->set_trunk_size(1.0);

  std::unique_ptr<Plant> plant2(
      plant_container_.AddPlant(kBeanTypeName, pos, *meteorology_));
  ASSERT_EQ(nullptr, plant2);

  Plant *find = plant_container_.GetPlant(pos);
  EXPECT_EQ(plant1.get(), find);
}

TEST_F(PlantContainerTest, DeletePlantTest) {
  std::unique_ptr<Plant> plant1(plant_container_.AddPlant(
      kBeanTypeName, Coordinate(5.0, 5.0), *meteorology_));
  ASSERT_NE(nullptr, plant1);
  plant1->set_trunk_size(1.0);

  std::unique_ptr<Plant> plant2(plant_container_.AddPlant(
      kBeanTypeName, Coordinate(0.0, 0.0), *meteorology_));
  ASSERT_NE(nullptr, plant2);
  plant2->set_trunk_size(1.0);

  auto position = plant1->position();
  EXPECT_FALSE(plant_container_.DelPlant({1.0, 1.0}));
  EXPECT_TRUE(plant_container_.DelPlant(position));
  EXPECT_EQ(nullptr, plant_container_.GetPlant(position));
  EXPECT_FALSE(plant_container_.DelPlant(position));

  position = plant2->position();
  EXPECT_TRUE(plant_container_.DelPlant(*plant2));
  EXPECT_EQ(nullptr, plant_container_.GetPlant(position));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
