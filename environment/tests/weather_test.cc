#include <chrono>

#include <gtest/gtest.h>

#include "weather.h"

using namespace environment;

TEST(WeatherTest, ConstructorTest_1) {
  Config config("place name", Location(100.0, 200.0, 300.0, 400.0));
  Climate climate(config);
  Weather weather(climate, std::chrono::system_clock::now());

  EXPECT_EQ(MaxMinTemperature({0, 0}), weather.temperature);
  EXPECT_EQ(0.0, weather.rainfall);
}

TEST(WeatherTest, ConstructorTest_2) {
  Weather weather(0.0, 1.0, 2.0);

  EXPECT_EQ(MaxMinTemperature({0.0, 1.0}), weather.temperature);
  EXPECT_EQ(2.0, weather.rainfall);
}

TEST(WeatherTest, ConstructorTest_3) {
  Weather weather(MaxMinTemperature({0.0, 1.0}), 2.0);

  EXPECT_EQ(MaxMinTemperature({0.0, 1.0}), weather.temperature);
  EXPECT_EQ(2.0, weather.rainfall);
}

TEST(WeatherTest, OperatorTest) {
  Weather lhs(0.0, 1.0, 2.0);
  Weather rhs(0.0, 1.0, 2.0);

  EXPECT_TRUE(lhs == rhs);

  Weather new_rhs(0.0, 1.0, 3.0);
  EXPECT_FALSE(lhs == new_rhs);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}