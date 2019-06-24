#include <iostream>

#include "gtest/gtest.h"

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "gtest_maind.lib")
#pragma comment(lib, "gtestd.lib")
#else
#pragma comment(lib, "gtest_main.lib")
#pragma comment(lib, "gtest.lib")
#endif
#endif
#include "environment/environment.h"
#include "simulators/sun_simulator.h"
using namespace simulator;
using namespace environment;

const double PI = 3.14159265359;
const size_t kTerrainSize = 5;

class SunSimulatorTest : public ::testing::Test {};

TEST_F(SunSimulatorTest, SimulateSunTest_6_22) {
  Config config("place name", Location(0, 0, 23.45, 23.45));
  Terrain terrain(kTerrainSize);
  Environment* env;
  struct tm tm;
  sscanf("2019-06-22 12:00:00", "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
         &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  auto time = std::chrono::system_clock::from_time_t(mktime(&tm));
  env = new Environment(config, time, terrain);
  struct SunInfo info;
  SunSimulator simulator;
  simulator.SimulateToTime(env, time);
  EXPECT_TRUE(env->sun_info().SolarAltitude <= 1);
  delete env;
}

TEST_F(SunSimulatorTest, SimulateSunTest_12_21) {
  Config config("place name", Location(0, 0, -23.45, -23.45));
  Terrain terrain(kTerrainSize);
  Environment* env;
  struct tm tm;
  sscanf("2019-12-21 12:00:00", "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
         &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  auto time = std::chrono::system_clock::from_time_t(mktime(&tm));
  env = new Environment(config, time, terrain);
  struct SunInfo info;
  SunSimulator simulator;
  simulator.SimulateToTime(env, time);
  EXPECT_TRUE(env->sun_info().SolarAltitude <= 1);
  delete env;
}

TEST_F(SunSimulatorTest, SimulateSunTest_3_21) {
  Config config("place name", Location(0, 0, 0, 0));
  Terrain terrain(kTerrainSize);
  Environment* env;
  struct tm tm;
  sscanf("2019-03-21 12:00:00", "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
         &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  auto time = std::chrono::system_clock::from_time_t(mktime(&tm));
  env = new Environment(config, time, terrain);
  struct SunInfo info;
  SunSimulator simulator;
  simulator.SimulateToTime(env, time);
  EXPECT_TRUE(env->sun_info().SolarAltitude <= 1);
  delete env;
}

TEST_F(SunSimulatorTest, SimulateSunTest_9_21) {
  Config config("place name", Location(0, 0, 0, 0));
  Terrain terrain(kTerrainSize);
  Environment* env;
  struct tm tm;
  sscanf("2019-09-21 12:00:00", "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
         &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  auto time = std::chrono::system_clock::from_time_t(mktime(&tm));
  env = new Environment(config, time, terrain);
  struct SunInfo info;
  SunSimulator simulator;
  simulator.SimulateToTime(env, time);
  EXPECT_TRUE(env->sun_info().SolarAltitude <= 1);
  delete env;
}
int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
