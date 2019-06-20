#include <string>

#include <gtest/gtest.h>

#include "agent/agent.h"
#include "environment.h"
#include "plant_types/plant_type.h"
#include "simulators/actions/crop.h"
#include "simulators/resource.h"

using namespace agent;
using namespace environment;
using namespace environment::plant_type;
using namespace simulator;
using namespace simulator::action;

class AgentTest : public ::testing::Test {
 public:
  ~AgentTest() {
    delete agent;
    delete env;
  }

 protected:
  void SetUp() override {
    Config config("place name", Location(100.0, 100.0, 200.0, 200.0));
    Terrain terrain(5);

    env = new Environment(config, std::chrono::system_clock::now(), terrain);
    agent = new Agent("agent name", env);
  }

  Agent* agent;
  Environment* env;
};

TEST_F(AgentTest, ConstructorTest_1) {
  std::string name("agent name");
  Agent agent(name, env);

  EXPECT_EQ(name, agent.name());
  EXPECT_EQ(env, agent.environment());
  EXPECT_TRUE(agent.owned_resource().empty());
}

TEST_F(AgentTest, ConstructorTest_2) {
  std::string name("agent name");
  std::unordered_map<ResourceType, size_t> resources;
  resources[ResourceType::LABOR] = 20;
  resources[ResourceType::MONEY] = 40;

  Agent agent(name, env, resources);

  EXPECT_EQ(name, agent.name());
  EXPECT_EQ(env, agent.environment());
  EXPECT_EQ(resources, agent.owned_resource());
}

TEST_F(AgentTest, ConstructorTest_3) {
  std::string name("agent name");
  std::unordered_map<ResourceType, size_t> resources;
  resources[ResourceType::LABOR] = 20;
  resources[ResourceType::MONEY] = 40;
  ResourceList resources_list;
  for (const auto& resource : resources) {
    resources_list.push_back(
        std::pair<ResourceType, size_t>(resource.first, resource.second));
  }

  Agent agent(name, env, resources_list);

  EXPECT_EQ(name, agent.name());
  EXPECT_EQ(env, agent.environment());
  EXPECT_EQ(resources, agent.owned_resource());
}

TEST_F(AgentTest, AddResourceTest) {
  agent->AddResource(ResourceType::MONEY, 100);

  auto find_money = agent->owned_resource().find(ResourceType::MONEY);
  ASSERT_NE(agent->owned_resource().end(), find_money);
  EXPECT_EQ(100, find_money->second);

  agent->AddResource(ResourceType::MONEY, 100);
  ASSERT_NE(agent->owned_resource().end(), find_money);
  EXPECT_EQ(200, find_money->second);
}

TEST_F(AgentTest, TakeActionTest) {
  // invalid argument
  auto ret_code = agent->TakeAction(nullptr);
  EXPECT_EQ(Agent::INVALID_ARGUMENT, ret_code);

  // not enough resources
  std::vector<std::pair<ResourceType, size_t>> cost;
  cost.push_back(std::pair<ResourceType, size_t>(ResourceType::MONEY, 10));
  const std::string kCornTypeName = "Corn";
  auto action = crop::Add(Coordinate(0, 0), std::chrono::system_clock::now(),
                          std::chrono::duration<int>(0), kCornTypeName, cost);
  ret_code = agent->TakeAction(&action);
  EXPECT_EQ(Agent::NOT_ENOUGH_RESOURCES, ret_code);

  // success
  agent->AddResource(ResourceType::MONEY, 100);
  ret_code = agent->TakeAction(&action);
  EXPECT_EQ(Agent::SUCCESS, ret_code);
  auto find_money = agent->owned_resource().find(ResourceType::MONEY);
  EXPECT_EQ(90, find_money->second);
}

struct FakePlantType : public PlantType {
  FakePlantType()
      : PlantType("Fake", "F", true, 0.0, MaxMinTemperature(0, 0),
                  MaxMinTemperature(0, 0), MaxMinRainfall(0, 0),
                  MaxMinRainfall(0, 0)) {}
  environment::Plant* GeneratePlantInstance() const override {
    return new Plant("Fake");
  }
};

TEST_F(AgentTest, GetQualifiedPlantsTest) {
  FakePlantType fake_plant_type;

  auto ret = agent->GetQualifiedPlants();

  ASSERT_FALSE(ret.empty());
  EXPECT_NE(ret.end(),
            std::find(ret.begin(), ret.end(), fake_plant_type.type_name));
}

TEST_F(AgentTest, GetOptimalPlantsTest) {
  FakePlantType fake_plant_type;

  auto ret = agent->GetOptimalPlants();

  ASSERT_FALSE(ret.empty());
  EXPECT_NE(ret.end(),
            std::find(ret.begin(), ret.end(), fake_plant_type.type_name));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
