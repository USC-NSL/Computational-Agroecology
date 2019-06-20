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
  Environment* ConstructDummyEnvironment() {
    Config config("place name", Location(100.0, 100.0, 200.0, 200.0));
    Terrain terrain(kTerrainSize);

    return new Environment(config, std::chrono::system_clock::now(), terrain);
  }

  void SetUp() override { env = ConstructDummyEnvironment(); }

  Environment* env;
};

TEST_F(EnvironmentTest, ConstructorTest) {
  Environment* tmp = ConstructDummyEnvironment();
  // Nothing happens means it should have been constructed successfully.
  delete tmp;
}

TEST_F(EnvironmentTest, JumpToTimeTest) {
  auto time = std::chrono::system_clock::now() + std::chrono::hours(1);
  env->JumpToTime(time);

  EXPECT_EQ(time, env->timestamp());
}

TEST_F(EnvironmentTest, JumpDurationTest) {
  auto current_time = env->timestamp();
  env->JumpDuration(std::chrono::hours(1));
  EXPECT_EQ(current_time + std::chrono::hours(1), env->timestamp());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
