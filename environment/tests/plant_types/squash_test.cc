#include <string>

#include <gtest/gtest.h>

#include "plant_types/squash.h"

using namespace environment::plant_type;

const std::string kSquashTypeName = "Squash";

TEST(SquashTest, GlobalVariableTest) {
  const Squash& squash =
      *(reinterpret_cast<const Squash*>(plant_type_to_plant[kSquashTypeName]));

  EXPECT_EQ(kSquashTypeName, squash.type_name);
  EXPECT_EQ("-", squash.display_symbol);
  EXPECT_EQ(true, squash.cultivar);

  EXPECT_FALSE(squash.optimal_soil_depth);
  EXPECT_FALSE(squash.absolute_soil_depth);

  EXPECT_FALSE(squash.base_temperature);
  EXPECT_FALSE(squash.optimal_temperature);
  EXPECT_FALSE(squash.absolute_temperature);

  EXPECT_FALSE(squash.optimal_annual_rainfall);
  EXPECT_FALSE(squash.absolute_annual_rainfall);

  EXPECT_FALSE(squash.optimal_soil_condition.fertility);
  EXPECT_FALSE(squash.optimal_soil_condition.salinity);
  EXPECT_FALSE(squash.optimal_soil_condition.pH);
  EXPECT_FALSE(squash.optimal_soil_condition.drainage);
  EXPECT_FALSE(squash.absolute_soil_condition.fertility);
  EXPECT_FALSE(squash.absolute_soil_condition.salinity);
  EXPECT_FALSE(squash.absolute_soil_condition.pH);
  EXPECT_FALSE(squash.absolute_soil_condition.drainage);

  EXPECT_FALSE(squash.optimal_latitude);
  EXPECT_FALSE(squash.absolute_latitude);

  EXPECT_FALSE(squash.optimal_light_condition.light_intensity);
  EXPECT_FALSE(squash.optimal_light_condition.angle);
  EXPECT_FALSE(squash.absolute_light_condition.light_intensity);
  EXPECT_FALSE(squash.absolute_light_condition.angle);

  EXPECT_FALSE(squash.climate_zone);

  EXPECT_FALSE(squash.photo_period);

  EXPECT_FALSE(squash.crop_cycle);
}

TEST(SquashTest, GenerateTest) {
  const Squash& squash =
      *(reinterpret_cast<const Squash*>(plant_type_to_plant[kSquashTypeName]));
  auto plant = squash.GeneratePlantInstance();

  EXPECT_EQ(kSquashTypeName, plant.type_name);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}