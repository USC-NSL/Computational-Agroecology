#include <chrono>

#include <gtest/gtest.h>

#include "agent_server/agent_server.h"
#include "environment/config.h"
#include "environment/location.h"
#include "environment/terrain.h"

using namespace agent_server;
using namespace environment;

const size_t kTerrainSize = 5;
const std::string kDummyEnvName = "dummy_env";
const std::chrono::system_clock::time_point kStartTime =
    std::chrono::system_clock::now();
const std::chrono::duration<int> kTimeStepLength = std::chrono::hours(1);
const std::string kDummyAgentName = "dummy_agent";
const size_t kRow = 5;
const size_t kCol = 6;

class AgentServerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    dummy_config =
        new Config("place name", Location(100.0, 100.0, 200.0, 200.0));
    dummy_terrain = new Terrain(kTerrainSize);

    agent_server.CreateEnvironment(kDummyEnvName, *dummy_config, kStartTime,
                                   kTimeStepLength, *dummy_terrain);
    agent_server.CreateQLearningAgent(kDummyAgentName, kDummyEnvName, kRow,
                                      kCol);
  }

  AgentServer agent_server;
  Config* dummy_config = nullptr;
  Terrain* dummy_terrain = nullptr;
};

TEST_F(AgentServerTest, CreateAndGetEnvironmentTest) {
  std::string env_name_1 = "env_name_1";
  auto create_ret = agent_server.CreateEnvironment(
      env_name_1, *dummy_config, kStartTime, kTimeStepLength, *dummy_terrain);
  EXPECT_EQ(AgentServer::OK, create_ret);

  create_ret = agent_server.CreateEnvironment(
      env_name_1, *dummy_config, kStartTime, kTimeStepLength, *dummy_terrain);
  EXPECT_EQ(AgentServer::ALREADY_EXISTS, create_ret);

  std::string env_name_2 = "env_name_2";
  create_ret = agent_server.CreateEnvironment(
      env_name_2, *dummy_config, kStartTime, kTimeStepLength, *dummy_terrain);
  EXPECT_EQ(AgentServer::OK, create_ret);

  std::string env_not_exist = "env_not_exist";
  auto get_ret_1 = agent_server.GetEnvironment(env_name_1);
  EXPECT_EQ(AgentServer::OK, get_ret_1.first);
  auto get_ret_2 = agent_server.GetEnvironment(env_name_2);
  EXPECT_EQ(AgentServer::OK, get_ret_2.first);
  auto get_ret_3 = agent_server.GetEnvironment(env_not_exist);
  EXPECT_EQ(AgentServer::ENV_NOT_FOUND, get_ret_3.first);
}

TEST_F(AgentServerTest, DeleteEnvironmentTest) {
  auto ret = agent_server.DeleteEnvironment(kDummyEnvName);
  EXPECT_EQ(AgentServer::OK, ret);

  ret = agent_server.DeleteEnvironment(kDummyEnvName);
  EXPECT_EQ(AgentServer::ENV_NOT_FOUND, ret);
}

TEST_F(AgentServerTest, CreateAgentTest) {
  std::string agent_name_1 = "agent_name_1";
  auto agent_env = agent_server.CreateQLearningAgent(agent_name_1,
                                                     kDummyEnvName, kRow, kCol);
  EXPECT_EQ(AgentServer::OK, agent_env);

  agent_env = agent_server.CreateQLearningAgent(agent_name_1, kDummyEnvName,
                                                kRow, kCol);
  EXPECT_EQ(AgentServer::ALREADY_EXISTS, agent_env);

  std::string env_not_exist = "env_not_exist";
  std::string agent_name_2 = "agent_name_2";
  agent_env = agent_server.CreateQLearningAgent(agent_name_2, env_not_exist,
                                                kRow, kCol);
  EXPECT_EQ(AgentServer::ENV_NOT_FOUND, agent_env);
}

TEST_F(AgentServerTest, DeleteAgentTest) {
  auto ret = agent_server.DeleteAgent(kDummyAgentName);
  EXPECT_EQ(AgentServer::OK, ret);

  ret = agent_server.DeleteAgent(kDummyAgentName);
  EXPECT_EQ(AgentServer::AGENT_NOT_FOUND, ret);
}

TEST_F(AgentServerTest, SimulateToTimeStepTest) {
  int64_t target_time_step = 123;
  auto ret = agent_server.SimulateToTimeStep("env_not_exist", target_time_step);
  EXPECT_EQ(AgentServer::ENV_NOT_FOUND, ret);

  ret = agent_server.SimulateToTimeStep(kDummyEnvName, target_time_step);
  EXPECT_EQ(AgentServer::OK, ret);

  auto env_ret = agent_server.GetEnvironment(kDummyEnvName);

  ASSERT_EQ(AgentServer::OK, env_ret.first);
  ASSERT_TRUE(env_ret.second.has_value());
  EXPECT_EQ(target_time_step, env_ret.second->time_step());
}

TEST_F(AgentServerTest, AgentTakeActionTest) {
  auto ret = agent_server.AgentTakeAction("agent_not_exist", nullptr);
  EXPECT_EQ(AgentServer::AGENT_NOT_FOUND, ret);

  // ignore the test on the action itself since it should pass its own test.

  environment::Coordinate coordinate(0, 0);
  std::string crop_type_name = "Corn";
  int64_t start_time_step = 1;
  int64_t duration = 1;
  agent::action::crop::Add action_without_cost(coordinate, start_time_step,
                                                   duration, crop_type_name);
  ret = agent_server.AgentTakeAction(kDummyAgentName, &action_without_cost);
  EXPECT_EQ(AgentServer::OK, ret);

  agent::Resources cost;
  cost[agent::ResourceType::LABOR] = 10;
  agent::action::crop::Add action_with_cost(coordinate, start_time_step,
                                                duration, cost, crop_type_name);
  ret = agent_server.AgentTakeAction(kDummyAgentName, &action_with_cost);
  EXPECT_EQ(AgentServer::ACTION_NOT_ENOUGH_RESOURCES, ret);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}