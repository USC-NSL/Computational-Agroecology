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
#include "environment.h"
#include "simulators/sun_simulator.h"
using namespace simulator;
using namespace environment;
const double PI = 3.14159265359;
class SunSimulatorTest : public ::testing::Test {};

TEST_F(SunSimulatorTest, SimulateSunTest) {
  SunSimulator simulator;
  struct SunInfo info1 =
      simulator.get_sunInfo_for_test(2019, 6, 21, 12, 0.0, 23.45);
  EXPECT_TRUE(info1.SolarAltitude <= 0.04);
  struct SunInfo info2 =
      simulator.get_sunInfo_for_test(2019, 12, 21, 12, 0.0, -23.45);
  EXPECT_TRUE(info2.SolarAltitude <= 0.04);
  struct SunInfo info3 =
      simulator.get_sunInfo_for_test(2019, 3, 21, 12, 0.0, 0.0);
  EXPECT_TRUE(info3.SolarAltitude <= 0.04);
  struct SunInfo info4 =
      simulator.get_sunInfo_for_test(2019, 9, 21, 12, 0.0, 0.0);
  EXPECT_TRUE(info4.SolarAltitude <= 0.04);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}