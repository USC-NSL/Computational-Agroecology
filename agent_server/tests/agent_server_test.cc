#include <chrono>

#include <gtest/gtest.h>

#include "agent_server.h"
#include "environment/config.h"
#include "environment/location.h"
#include "environment/terrain.h"

using namespace agent_server;
using namespace environment;

const size_t kTerrainSize = 5;
const std::string kDummyEnvName = "dummy_env";
const std::string kDummyAgentName = "dummy_agent";

class AgentServerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    dummy_config =
        new Config("place name", Location(100.0, 100.0, 200.0, 200.0));
    dummy_terrain = new Terrain(kTerrainSize);

    agent_server.CreateEnvironment(kDummyEnvName, *dummy_config,
                                   std::chrono::system_clock::now(),
                                   *dummy_terrain);
    agent_server.CreateAgent(kDummyAgentName, kDummyEnvName);
  }

  AgentServer agent_server;
  Config* dummy_config = nullptr;
  Terrain* dummy_terrain = nullptr;
};

TEST_F(AgentServerTest, CreateAndGetEnvironmentTest) {
  std::string env_name_1 = "env_name_1";
  auto create_ret = agent_server.CreateEnvironment(
      env_name_1, *dummy_config, std::chrono::system_clock::now(),
      *dummy_terrain);
  EXPECT_EQ(AgentServer::OK, create_ret);

  create_ret = agent_server.CreateEnvironment(env_name_1, *dummy_config,
                                              std::chrono::system_clock::now(),
                                              *dummy_terrain);
  EXPECT_EQ(AgentServer::ALREADY_EXISTS, create_ret);

  std::string env_name_2 = "env_name_2";
  create_ret = agent_server.CreateEnvironment(env_name_2, *dummy_config,
                                              std::chrono::system_clock::now(),
                                              *dummy_terrain);
  EXPECT_EQ(AgentServer::OK, create_ret);

  std::string env_not_exist = "env_not_exist";
  auto get_ret_1 = agent_server.GetEnvironment(env_name_1);
  EXPECT_EQ(AgentServer::OK, get_ret_1.first);
  auto get_ret_2 = agent_server.GetEnvironment(env_name_2);
  EXPECT_EQ(AgentServer::OK, get_ret_2.first);
  auto get_ret_3 = agent_server.GetEnvironment(env_not_exist);
  EXPECT_EQ(AgentServer::ENV_NOT_FOUND, get_ret_3.first);
}

TEST_F(AgentServerTest, CreateAgentTest) {
  std::string agent_name_1 = "agent_name_1";
  auto agent_env = agent_server.CreateAgent(agent_name_1, kDummyEnvName);
  EXPECT_EQ(AgentServer::OK, agent_env);

  agent_env = agent_server.CreateAgent(agent_name_1, kDummyEnvName);
  EXPECT_EQ(AgentServer::ALREADY_EXISTS, agent_env);

  std::string env_not_exist = "env_not_exist";
  std::string agent_name_2 = "agent_name_2";
  agent_env = agent_server.CreateAgent(agent_name_2, env_not_exist);
  EXPECT_EQ(AgentServer::ENV_NOT_FOUND, agent_env);
}

TEST_F(AgentServerTest, SimulateToTimeTest) {
  auto ret = agent_server.SimulateToTime("env_not_exist",
                                         std::chrono::system_clock::now());
  EXPECT_EQ(AgentServer::ENV_NOT_FOUND, ret);

  auto time = std::chrono::system_clock::now();
  ret = agent_server.SimulateToTime(kDummyEnvName, time);
  EXPECT_EQ(AgentServer::OK, ret);

  auto env_ret = agent_server.GetEnvironment(kDummyEnvName);

  ASSERT_EQ(AgentServer::OK, env_ret.first);
  ASSERT_TRUE(env_ret.second.has_value());
  EXPECT_EQ(time, env_ret.second->timestamp());
}

TEST_F(AgentServerTest, AgentTakeActionTest) {
  auto ret = agent_server.AgentTakeAction("agent_not_exist", nullptr);
  EXPECT_EQ(AgentServer::AGENT_NOT_FOUND, ret);

  // ignore the test on the action itself since it should pass its own test.

  environment::Coordinate coordinate(0, 0);
  std::string crop_type_name = "Corn";
  simulator::action::crop::Add action_without_cost(
      coordinate, std::chrono::system_clock::now(), std::chrono::hours(1),
      crop_type_name);
  ret = agent_server.AgentTakeAction(kDummyAgentName, &action_without_cost);
  EXPECT_EQ(AgentServer::OK, ret);

  std::vector<std::pair<simulator::ResourceType, size_t>> cost;
  cost.push_back(std::make_pair(simulator::ResourceType::LABOR, 10));
  simulator::action::crop::Add action_with_cost(
      coordinate, std::chrono::system_clock::now(), std::chrono::hours(1),
      crop_type_name, cost);
  ret = agent_server.AgentTakeAction(kDummyAgentName, &action_with_cost);
  EXPECT_EQ(AgentServer::ACTION_NOT_ENOUGH_RESOURCES, ret);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}