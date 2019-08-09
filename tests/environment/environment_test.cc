#include <chrono>
#include <vector>

#include <gtest/gtest.h>

#include "agent/actions/action.h"
#include "agent/actions/crop.h"
#include "environment/environment.h"

using namespace agent::action;
using namespace environment;

class EnvironmentTest : public ::testing::Test {
 public:
  ~EnvironmentTest() { delete env; }

 protected:
  Environment *ConstructDummyEnvironment() {
    auto time = std::chrono::system_clock::now();
    auto time_step_length = std::chrono::hours(1);

    Config config("place name", Location(100.0, 100.0, 200.0, 200.0));
    Terrain terrain(kTerrainSize);

    return new Environment(config, time, time_step_length, terrain);
  }

  void SetUp() override {
    env = ConstructDummyEnvironment();

    const std::string kBeanTypeName = "bean";
    int64_t time_step = 0;
    int64_t duration = 1;
    for (size_t i = 0; i < kNumberOfActions; ++i) {
      actions.push_back(
          new crop::Add(Coordinate(i, i), time_step, duration, kBeanTypeName));
      time_step += 2;
    }
  }

  const size_t kNumberOfActions = 5;
  const size_t kTerrainSize = kNumberOfActions;
  Environment *env;
  std::vector<const Action *> actions;
};

TEST_F(EnvironmentTest, ConstructorTest) {
  Environment *tmp = ConstructDummyEnvironment();
  // Nothing happens means it should have been constructed successfully.
  delete tmp;
}

TEST_F(EnvironmentTest, JumpToTimeStepTest) {
  if (env->action_pq().empty()) {
    env->ReceiveActions(actions);
  }

  int64_t now_time_step = 0;
  for (size_t i = 0; i < kNumberOfActions; ++i, now_time_step += 2) {
    // the ith action should have started
    env->JumpToTimeStep(now_time_step);
    ASSERT_EQ(now_time_step, env->time_step());

    EXPECT_EQ(kNumberOfActions - i - 1, env->action_pq().size());
    EXPECT_EQ(1, env->starting_action_pq().size());

    // the ith action should have completed
    env->JumpToTimeStep(now_time_step + 1);
    ASSERT_EQ(now_time_step + 1, env->time_step());
    ASSERT_EQ(kNumberOfActions - i - 1, env->action_pq().size());
    ASSERT_EQ(0, env->starting_action_pq().size());

    // since the action has completed
    // we should see its effect in the `terrain`
    EXPECT_NE(nullptr, env->GetPlant(Coordinate(i, i)));
  }
}

TEST_F(EnvironmentTest, JumpForwardTimeTest) {
  auto current_time_step = env->time_step();
  env->JumpForwardTimeStep(1);
  EXPECT_EQ(current_time_step + 1, env->time_step());
}

TEST_F(EnvironmentTest, ReceiveActionsTest) {
  size_t original_action_size = env->action_pq().size();
  env->ReceiveActions(actions);

  // should see the change in size of the internal pq
  EXPECT_EQ(original_action_size + actions.size(), env->action_pq().size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

