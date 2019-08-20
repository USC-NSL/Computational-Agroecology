#include <chrono>

#include <gtest/gtest.h>

#include "environment/weather.h"

using namespace environment;

TEST(WeatherTest, ConstructorTest_1) {
  Location location(100.0, 200.0, 300.0, 400.0);
  Weather weather(location, std::chrono::system_clock::now());

  EXPECT_EQ(0.0f, weather.total_sunshine_hour);
  EXPECT_EQ(MinMaxTemperature({0, 0}), weather.air_temperature);
  EXPECT_EQ(0.0f, weather.relative_humidity);
  EXPECT_EQ(0.0f, weather.wind_speed);
  EXPECT_EQ(0.0f, weather.rainfall);
}

TEST(WeatherTest, ConstructorTest_2) {
  Weather weather(0.0, 1.0, 2.0, 3.0, 4.0, 5.0);

  EXPECT_EQ(0.0, weather.total_sunshine_hour);
  EXPECT_EQ(MinMaxTemperature({1.0, 2.0}), weather.air_temperature);
  EXPECT_EQ(3.0, weather.relative_humidity);
  EXPECT_EQ(4.0, weather.wind_speed);
  EXPECT_EQ(5.0, weather.rainfall);
}

TEST(WeatherTest, ConstructorTest_3) {
  Weather weather(0.0, MinMaxTemperature({1.0, 2.0}), 3.0, 4.0, 5.0);

  EXPECT_EQ(0.0, weather.total_sunshine_hour);
  EXPECT_EQ(MinMaxTemperature({1.0, 2.0}), weather.air_temperature);
  EXPECT_EQ(3.0, weather.relative_humidity);
  EXPECT_EQ(4.0, weather.wind_speed);
  EXPECT_EQ(5.0, weather.rainfall);
}

TEST(WeatherTest, OperatorTest) {
  Weather lhs(0.0, 1.0, 2.0, 3.0, 4.0, 5.0);
  Weather rhs(0.0, 1.0, 2.0, 3.0, 4.0, 5.0);

  EXPECT_TRUE(lhs == rhs);

  Weather new_rhs(0.0, 1.0, 2.0, 3.0, 4.0, 6.0);
  EXPECT_FALSE(lhs == new_rhs);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}