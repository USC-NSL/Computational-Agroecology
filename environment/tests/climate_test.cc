

#include <gtest/gtest.h>

#include "climate.h"

using namespace environment;

TEST(ClimateConstructorTest, Constructor_1) {
  Location location(100, 100, 200, 200);
  Config config("name", location);

  Climate climate(config);

  EXPECT_EQ(0, climate.yearly_temperature.max);
  EXPECT_EQ(0, climate.yearly_temperature.min);
  EXPECT_EQ(0, climate.yearly_rainfall.max);
  EXPECT_EQ(0, climate.yearly_rainfall.min);
  EXPECT_EQ(Climate::TropicalWerAndDry, climate.climate_zone);
}

TEST(ClimateConstructorTest, Constructor_2) {
  Climate climate(1.0, 2.0, 3.0, 4.0, Climate::Polar);

  EXPECT_EQ(1.0, climate.yearly_temperature.max);
  EXPECT_EQ(2.0, climate.yearly_temperature.min);
  EXPECT_EQ(3.0, climate.yearly_rainfall.max);
  EXPECT_EQ(4.0, climate.yearly_rainfall.min);
  EXPECT_EQ(Climate::Polar, climate.climate_zone);
}

TEST(ClimateConstructorTest, Constructor_3) {
  MaxMinTemperature yearly_temp(1.0, 2.0);
  MaxMinRainfall yearly_rainfall(3.0, 4.0);

  Climate climate(yearly_temp, yearly_rainfall, Climate::Polar);

  EXPECT_EQ(1.0, climate.yearly_temperature.max);
  EXPECT_EQ(2.0, climate.yearly_temperature.min);
  EXPECT_EQ(3.0, climate.yearly_rainfall.max);
  EXPECT_EQ(4.0, climate.yearly_rainfall.min);
  EXPECT_EQ(Climate::Polar, climate.climate_zone);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
