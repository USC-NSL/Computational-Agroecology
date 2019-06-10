#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "environment.h"
#include "simulators/actions/crop.h"
#include "simulators/main_simulator.h"

using namespace environment;
using namespace simulator;
using namespace simulator::action;

const size_t kNumberOfActions = 5;
const size_t kTerrainSize = kNumberOfActions;

class MainSimulatorTest : public ::testing::Test {
 public:
  ~MainSimulatorTest() {
    for (const auto action : actions) {
      delete action;
    }
    delete env;
  }

 protected:
  void SetUp() override {
    auto time = std::chrono::system_clock::now();

    Config config("place name", Location(100.0, 100.0, 200.0, 200.0));
    Terrain terrain(kTerrainSize);
    env = new Environment(config, time, terrain);

    const std::string kCornTypeName = "Corn";
    auto duration = std::chrono::minutes(5);
    for (size_t i = 0; i < kNumberOfActions; ++i) {
      actions.push_back(
          new crop::Add(Coordinate(i, i), time, duration, kCornTypeName));
      time += std::chrono::minutes(10);
    }
  }

  Environment* env;
  MainSimulator simulator;
  std::vector<const Action*> actions;
};

TEST_F(MainSimulatorTest, ReceiveActionsTest) {
  size_t original_action_size = simulator.action_pq().size();
  simulator.ReceiveActions(actions);

  // should see the change in size of the internal pq
  EXPECT_EQ(original_action_size + actions.size(),
            simulator.action_pq().size());
}

TEST_F(MainSimulatorTest, SimulateToTimeTest) {
  simulator.ReceiveActions(actions);

  // jump to 2 minutes later
  auto now_time = env->timestamp() + std::chrono::minutes(2);
  for (size_t i = 0; i < kNumberOfActions;
       ++i, now_time += std::chrono::minutes(10)) {
    // the ith action should have started
    simulator.SimulateToTime(env, now_time);

    EXPECT_EQ(kNumberOfActions - i - 1, simulator.action_pq().size());
    EXPECT_EQ(1, simulator.pending_action_pq().size());

    // jump to 5 minutes later
    // the ith action should have completed
    simulator.SimulateToTime(env, now_time + std::chrono::minutes(5));
    ASSERT_EQ(kNumberOfActions - i - 1, simulator.action_pq().size());
    ASSERT_EQ(0, simulator.pending_action_pq().size());

    // since the action has completed
    // we should see its effect in the `terrain`
    EXPECT_NE(nullptr, env->terrain().tiles()[i][i].plant);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
