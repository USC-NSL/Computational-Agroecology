#include <chrono>
#include <vector>

#include <gtest/gtest.h>

#include "environment.h"
#include "simulators/actions/crop.h"

using namespace environment;

const size_t kTerrainSize = 5;

class EnvironmentTest : public ::testing::Test {
 public:
  ~EnvironmentTest() { delete env; }

 protected:
  Environment *ConstructDummyEnvironment() {
    Config config("place name", Location(100.0, 100.0, 200.0, 200.0));
    Terrain terrain(kTerrainSize);

    return new Environment(config, std::chrono::system_clock::now(),
                           std::chrono::hours(1), terrain);
  }

  void SetUp() override { env = ConstructDummyEnvironment(); }

  Environment *env;
};

TEST_F(EnvironmentTest, ConstructorTest) {
  Environment *tmp = ConstructDummyEnvironment();
  // Nothing happens means it should have been constructed successfully.
  delete tmp;
}

TEST_F(EnvironmentTest, JumpToTimeStepTest) {
  env->JumpToTimeStep(1);

  EXPECT_EQ(1, env->time_step());
}

TEST_F(EnvironmentTest, JumpForwardTimeTest) {
  auto current_time_step = env->time_step();
  env->JumpForwardTimeStep(1);
  EXPECT_EQ(current_time_step + 1, env->time_step());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
