#include <iostream>
#include <gtest/gtest.h>
#include "../../simulators/sun_simulator.h"
#include "../../environment.h"

using namespace simulator;
using namespace environment;
const double PI = 3.14159265359;
class SunSimulatorTest : public ::testing::Test {
};

TEST_F(SunSimulatorTest, SimulateSunTest) {
	SunSimulator simulator;
	struct SunInfo info1= simulator.get_sunInfo_for_test(2019, 6, 21, 12, 0.0, 23.45);
	EXPECT_TRUE((PI / 2.0 - info1._solarAltitude) <= 0.02);
	struct SunInfo info2 = simulator.get_sunInfo_for_test(2019, 12, 21, 12, 0.0, -23.45);
	EXPECT_TRUE((PI / 2.0 - info2._solarAltitude) <= 0.02);
	struct SunInfo info3 = simulator.get_sunInfo_for_test(2019, 3, 22, 12, 0.0, 0.0);
	EXPECT_TRUE((PI / 2.0 - info3._solarAltitude) <= 0.02);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
