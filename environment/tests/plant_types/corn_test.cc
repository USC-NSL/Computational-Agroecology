

#include <gtest/gtest.h>

#include "plant_types/corn.h"

using namespace environment::plant_type;

TEST(CornTest, GlobalVariableTest) {
  const Corn& corn =
      *(reinterpret_cast<const Corn*>(plant_type_to_plant[type_id::CORN]));

  EXPECT_EQ(type_id::CORN, corn.id);
  EXPECT_EQ("corn", corn.name);
  EXPECT_EQ("🌽", corn.display_symbol);
  EXPECT_EQ(true, corn.cultivar);
  EXPECT_EQ(0.0, corn.base_temperature);
  EXPECT_EQ(MaxMinTemperature(0, 0), corn.optimal_temperature);
  EXPECT_EQ(MaxMinTemperature(0, 0), corn.absolute_temperature);
  EXPECT_EQ(MaxMinRainfall(0, 0), corn.optimal_annual_rainfall);
  EXPECT_EQ(MaxMinRainfall(0, 0), corn.absolute_annual_rainfall);
}

TEST(CornTest, GenerateTest) {
  const Corn& corn =
      *(reinterpret_cast<const Corn*>(plant_type_to_plant[type_id::CORN]));
  auto plant = corn.GeneratePlantInstance();

  EXPECT_EQ(type_id::CORN, plant->type);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}