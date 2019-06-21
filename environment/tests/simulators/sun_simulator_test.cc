#include <iostream>
#include <gtest/gtest.h>
#include "../../simulators/sun_simulator.h"
#include "../../environment.h"

using namespace simulator;
using namespace environment;
const size_t kTerrainSize = 5;

class SunSimulatorTest : public ::testing::Test {
protected:
  void SetUp() override {
    auto time = std::chrono::system_clock::now();

    Config config("place name", Location(100.0, 100.0, 200.0, 200.0));
    Terrain terrain(kTerrainSize);
    env = new Environment(config, time, terrain);
  }
private:
  Environment* env;
  SunSimulator simulator;
};

TEST_F(SunSimulatorTest, SimulateToTimeTest) {
  for (size_t i = 0; i < 5; i++) {
    auto now_time = env->timestamp() + std::chrono::hours(1);
    simulator.SimulateToTime(env, time);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
