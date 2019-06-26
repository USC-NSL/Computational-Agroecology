#include <chrono>

#include <gtest/gtest.h>

#include "agent_server.h"
#include "environment/config.h"
#include "environment/location.h"
#include "environment/terrain.h"

using namespace agent_server;
using namespace environment;

const size_t kTerrainSize = 5;

class AgentServerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    dummy_config =
        new Config("place name", Location(100.0, 100.0, 200.0, 200.0));
    dummy_terrain = new Terrain(kTerrainSize);
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
  EXPECT_EQ(AgentServer::NOT_FOUND, get_ret_3.first);
}

TEST_F(AgentServerTest, CreateAgentTest) {
  std::string env_name = "env_name";
  auto env_ret = agent_server.CreateEnvironment(
      env_name, *dummy_config, std::chrono::system_clock::now(),
      *dummy_terrain);
  ASSERT_EQ(AgentServer::OK, env_ret);

  std::string agent_name_1 = "agent_name_1";
  auto agent_env = agent_server.CreateAgent(agent_name_1, env_name);
  EXPECT_EQ(AgentServer::OK, agent_env);

  agent_env = agent_server.CreateAgent(agent_name_1, env_name);
  EXPECT_EQ(AgentServer::ALREADY_EXISTS, agent_env);

  std::string env_not_exist = "env_not_exist";
  std::string agent_name_2 = "agent_name_2";
  agent_env = agent_server.CreateAgent(agent_name_2, env_not_exist);
  EXPECT_EQ(AgentServer::NOT_FOUND, agent_env);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}