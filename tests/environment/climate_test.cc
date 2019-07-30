#include <gtest/gtest.h>

#include "environment/climate.h"

using namespace environment;

TEST(ClimateTest, ConstructorTest_1) {
  Location location(100, 100, 200, 200);
  Config config("name", location);

  Climate climate(config);

  EXPECT_EQ(0, climate.yearly_temperature.min);
  EXPECT_EQ(0, climate.yearly_temperature.max);
  EXPECT_EQ(0, climate.yearly_rainfall.min);
  EXPECT_EQ(0, climate.yearly_rainfall.max);
  EXPECT_EQ(Climate::TropicalWetAndDry, climate.climate_zone);
}

TEST(ClimateTest, ConstructorTest_2) {
  Climate climate(1.0, 2.0, 3.0, 4.0, Climate::Polar);

  EXPECT_EQ(1.0, climate.yearly_temperature.min);
  EXPECT_EQ(2.0, climate.yearly_temperature.max);
  EXPECT_EQ(3.0, climate.yearly_rainfall.min);
  EXPECT_EQ(4.0, climate.yearly_rainfall.max);
  EXPECT_EQ(Climate::Polar, climate.climate_zone);
}

TEST(ClimateTest, ConstructorTest_3) {
  MinMaxTemperature yearly_temp(1.0, 2.0);
  MinMaxRainfall yearly_rainfall(3.0, 4.0);

  Climate climate(yearly_temp, yearly_rainfall, Climate::Polar);

  EXPECT_EQ(1.0, climate.yearly_temperature.min);
  EXPECT_EQ(2.0, climate.yearly_temperature.max);
  EXPECT_EQ(3.0, climate.yearly_rainfall.min);
  EXPECT_EQ(4.0, climate.yearly_rainfall.max);
  EXPECT_EQ(Climate::Polar, climate.climate_zone);
}

TEST(ClimateTest, OperatorTest) {
  Climate lhs(1.0, 2.0, 3.0, 4.0, Climate::Polar);
  Climate rhs(1.0, 2.0, 3.0, 4.0, Climate::Polar);

  EXPECT_TRUE(lhs == rhs);

  Climate new_rhs(1.0, 2.0, 3.0, 4.0, Climate::DesertOrArid);
  EXPECT_FALSE(lhs == new_rhs);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
