#include <iostream>
#include <memory>

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
#include "environment.h"
#include "simulators/sun_simulator.h"
using namespace simulator;
using namespace environment;

const double PI = 3.14159265359;
const size_t kTerrainSize = 5;

class SunSimulatorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    terrain.reset(new Terrain(kTerrainSize));
    time_step_length = std::chrono::hours(1);
  }

  std::unique_ptr<Terrain> terrain;
  std::unique_ptr<Environment> env;
  std::chrono::duration<int> time_step_length;
};

TEST_F(SunSimulatorTest, SimulateSunTest_6_22) {
  Config config("place name", Location(0, 0, 23.45, 23.45));
  struct tm tm;
  sscanf("2019-06-22 12:00:00", "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
         &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  auto time = std::chrono::system_clock::from_time_t(mktime(&tm));
  env.reset(new Environment(config, time, time_step_length, *terrain));
  struct SunInfo info;
  SunSimulator simulator;
  simulator.SimulateToTime(env.get(), time);
  EXPECT_TRUE(env->sun_info().SolarAltitude <= 1);
}

TEST_F(SunSimulatorTest, SimulateSunTest_12_21) {
  Config config("place name", Location(0, 0, -23.45, -23.45));
  struct tm tm;
  sscanf("2019-12-21 12:00:00", "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
         &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  auto time = std::chrono::system_clock::from_time_t(mktime(&tm));
  env.reset(new Environment(config, time, time_step_length, *terrain));
  struct SunInfo info;
  SunSimulator simulator;
  simulator.SimulateToTime(env.get(), time);
  EXPECT_TRUE(env->sun_info().SolarAltitude <= 1);
}

TEST_F(SunSimulatorTest, SimulateSunTest_3_21) {
  Config config("place name", Location(0, 0, 0, 0));
  struct tm tm;
  sscanf("2019-03-21 12:00:00", "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
         &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  auto time = std::chrono::system_clock::from_time_t(mktime(&tm));
  env.reset(new Environment(config, time, time_step_length, *terrain));
  struct SunInfo info;
  SunSimulator simulator;
  simulator.SimulateToTime(env.get(), time);
  EXPECT_TRUE(env->sun_info().SolarAltitude <= 1);
}

TEST_F(SunSimulatorTest, SimulateSunTest_9_21) {
  Config config("place name", Location(0, 0, 0, 0));
  struct tm tm;
  sscanf("2019-09-21 12:00:00", "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
         &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  auto time = std::chrono::system_clock::from_time_t(mktime(&tm));
  env.reset(new Environment(config, time, time_step_length, *terrain));
  struct SunInfo info;
  SunSimulator simulator;
  simulator.SimulateToTime(env.get(), time);
  EXPECT_TRUE(env->sun_info().SolarAltitude <= 1);
}
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
