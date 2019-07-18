#include <algorithm>
#include <string>

#include <gtest/gtest.h>

#include "agent/agent.h"
#include "agent/q_learning.h"
#include "environment.h"
#include "simulators/actions/crop.h"

using namespace agent;
using namespace environment;
using namespace simulator;
using namespace simulator::action;


class QlearningTest : public ::testing::Test {
 public:
  ~QlearningTest() {
    delete qlearning;
    delete env;
  }

 protected:
  void SetUp() override {
    Config config("place name", Location(100.0, 100.0, 200.0, 200.0));
    Terrain terrain(5);

    env = new Environment(config, std::chrono::system_clock::now(),
                          std::chrono::hours(1), terrain);
    qlearning = new Qlearning("agent name", env, 10, 54);
  }

  Qlearning* qlearning;
  Environment* env;
};

TEST_F(QlearningTest, ConstructorTest_1) {
  std::string name("agent name");
  Qlearning agent(name, env, 10, 54);

  EXPECT_EQ(name, agent.name());
  EXPECT_EQ(env, agent.environment());
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
