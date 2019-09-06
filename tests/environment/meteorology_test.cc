#include <chrono>
#include <cstdio>
#include <memory>

#include <gtest/gtest.h>

#include "environment/meteorology.h"

using namespace environment;

class MeteorologyTest : public ::testing::Test {
 protected:
  void SetUp() override {
    weather_ = std::make_shared<Weather>(0.0, 26.0, 32.0, 0.0, 0.0, 0.0);
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

  const double kOneDegreeInRadians = 1.0 * kPI / kPIforDegree;
};

TEST_F(MeteorologyTest, On_06_21) {
  Location location(0, 0, 23.45, 23.45);
  auto tp = CreateTimePoint(2019, 6, 21, 12, 0, 0);

  Meteorology meteorology(tp, location, climate_zone_, *weather_);

  std::cout << "Solar inclination: "
            << meteorology.solar_inclination() * kPIforDegree / kPI
            << " degree(s).\n";
  // TODO: figure the expected value
  EXPECT_LE(meteorology.solar_inclination(), kOneDegreeInRadians);
}

TEST_F(MeteorologyTest, On_12_21) {
  Location location(0, 0, -23.45, -23.45);
  auto tp = CreateTimePoint(2019, 12, 21, 12, 0, 0);

  Meteorology meteorology(tp, location, climate_zone_, *weather_);

  std::cout << "Solar inclination: "
            << meteorology.solar_inclination() * kPIforDegree / kPI
            << " degree(s).\n";
  // TODO: figure the expected value
  EXPECT_LE(meteorology.solar_inclination(), kOneDegreeInRadians);
}

TEST_F(MeteorologyTest, On_03_21) {
  Location location(0, 0, 0, 0);
  auto tp = CreateTimePoint(2019, 3, 21, 12, 0, 0);

  Meteorology meteorology(tp, location, climate_zone_, *weather_);

  std::cout << "Solar inclination: "
            << meteorology.solar_inclination() * kPIforDegree / kPI
            << " degree(s).\n";
  // TODO: figure the expected value
  EXPECT_LE(meteorology.solar_inclination(), kOneDegreeInRadians);
}

TEST_F(MeteorologyTest, On_09_21) {
  Location location(0, 0, 0, 0);
  auto tp = CreateTimePoint(2019, 9, 21, 12, 0, 0);

  Meteorology meteorology(tp, location, climate_zone_, *weather_);

  std::cout << "Solar inclination: "
            << meteorology.solar_inclination() * kPIforDegree / kPI
            << " degree(s).\n";
  // TODO: figure the expected value
  EXPECT_LE(meteorology.solar_inclination(), kOneDegreeInRadians);
}

// TODO: add more tests

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
