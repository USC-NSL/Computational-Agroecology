#include <chrono>
#include <cstdio>
#include <memory>

#include <gtest/gtest.h>

#include "environment/meteo_info.h"

using namespace environment;

class MeteoInfoTest : public ::testing::Test {
 protected:
  void SetUp() override {
    weather_ = std::make_shared<Weather>(26.0, 32.0, 0.0);
  }

  std::chrono::system_clock::time_point CreateTimePoint(
      const int year, const int month, const int day, const int hour,
      const int minute, const int second) {
    struct tm tm;
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;

    return std::chrono::system_clock::from_time_t(mktime(&tm));
  }

  // just randomly pick one
  Climate::ZoneType climate_zone_ = Climate::TemperateOceanic;

  // dummy weather
  std::shared_ptr<Weather> weather_;
};

TEST_F(MeteoInfoTest, On_06_21) {
  Location location(0, 0, 23.45, 23.45);
  auto tp = CreateTimePoint(2019, 6, 21, 12, 0, 0);

  MeteoInfo meteo_info(tp, location, climate_zone_, *weather_);

  // TODO: figure the expected value
  EXPECT_LE(meteo_info.solar_azimuth(), 1);
}

TEST_F(MeteoInfoTest, On_12_21) {
  Location location(0, 0, -23.45, -23.45);
  auto tp = CreateTimePoint(2019, 12, 21, 12, 0, 0);

  MeteoInfo meteo_info(tp, location, climate_zone_, *weather_);

  // TODO: figure the expected value
  EXPECT_LE(meteo_info.solar_azimuth(), 1);
}

TEST_F(MeteoInfoTest, On_03_21) {
  Location location(0, 0, 0, 0);
  auto tp = CreateTimePoint(2019, 3, 21, 12, 0, 0);

  MeteoInfo meteo_info(tp, location, climate_zone_, *weather_);

  // TODO: figure the expected value
  EXPECT_LE(meteo_info.solar_azimuth(), 1);
}

TEST_F(MeteoInfoTest, On_09_21) {
  Location location(0, 0, 0, 0);
  auto tp = CreateTimePoint(2019, 9, 21, 12, 0, 0);

  MeteoInfo meteo_info(tp, location, climate_zone_, *weather_);

  // TODO: figure the expected value
  EXPECT_LE(meteo_info.solar_azimuth(), 1);
}

// TODO: add more tests

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
