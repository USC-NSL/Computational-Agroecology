#include <string>

#include <gtest/gtest.h>

#include "plant_types/corn.h"

using namespace environment::plant_type;

const std::string kCornTypeName = "Corn";

TEST(CornTest, GlobalVariableTest) {
  const Corn& corn =
      *(reinterpret_cast<const Corn*>(plant_type_to_plant[kCornTypeName]));

  EXPECT_EQ(kCornTypeName, corn.type_name);
  EXPECT_EQ("🌽", corn.display_symbol);
  EXPECT_EQ(true, corn.cultivar);

  EXPECT_FALSE(corn.optimal_soil_depth);
  EXPECT_FALSE(corn.absolute_soil_depth);

  EXPECT_FALSE(corn.base_temperature);
  EXPECT_FALSE(corn.optimal_temperature);
  EXPECT_FALSE(corn.absolute_temperature);

  EXPECT_FALSE(corn.optimal_annual_rainfall);
  EXPECT_FALSE(corn.absolute_annual_rainfall);

  EXPECT_FALSE(corn.optimal_soil_condition.fertility);
  EXPECT_FALSE(corn.optimal_soil_condition.salinity);
  EXPECT_FALSE(corn.optimal_soil_condition.pH);
  EXPECT_FALSE(corn.optimal_soil_condition.drainage);
  EXPECT_FALSE(corn.absolute_soil_condition.fertility);
  EXPECT_FALSE(corn.absolute_soil_condition.salinity);
  EXPECT_FALSE(corn.absolute_soil_condition.pH);
  EXPECT_FALSE(corn.absolute_soil_condition.drainage);

  EXPECT_FALSE(corn.optimal_latitude);
  EXPECT_FALSE(corn.absolute_latitude);

  EXPECT_FALSE(corn.optimal_light_condition.light_intensity);
  EXPECT_FALSE(corn.optimal_light_condition.angle);
  EXPECT_FALSE(corn.absolute_light_condition.light_intensity);
  EXPECT_FALSE(corn.absolute_light_condition.angle);

  EXPECT_FALSE(corn.climate_zone);

  EXPECT_FALSE(corn.photo_period);

  EXPECT_FALSE(corn.crop_cycle);
}

TEST(CornTest, GenerateTest) {
  const Corn& corn =
      *(reinterpret_cast<const Corn*>(plant_type_to_plant[kCornTypeName]));
  auto plant = corn.GeneratePlantInstance();

  EXPECT_EQ(kCornTypeName, plant.type_name);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}