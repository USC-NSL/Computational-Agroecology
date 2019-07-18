#include <algorithm>
#include <string>

#include <gtest/gtest.h>

#include "agent/agent.h"
#include "agent/q_learning.h"
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
    agent = new Qlearning("agent name", env, 10, 54);
  }

  Agent* agent;
  Environment* env;
};

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

const environment::SoilCondition kFakeSoilCondition(std::nullopt, std::nullopt,
                                                    std::nullopt, std::nullopt);
const environment::Light kFakeLight(
    MinMaxPair<Light::LightIntensity>(
        environment::Light::kLightIntensityVeryBright,
        environment::Light::kLightIntensityVeryBright),
    1.1);

struct FakePlantType : public PlantType {
  FakePlantType()
      : PlantType("Fake",              // type_name
                  "F",                 // display_symbol
                  true,                // cultivar
                  std::nullopt,        // optimal_soil_depth
                  std::nullopt,        // absolute_soil_depth
                  std::nullopt,        // base_temperature
                  std::nullopt,        // optimal_temperature
                  std::nullopt,        // absolute_temperature
                  std::nullopt,        // optimal_annual_rainfall
                  std::nullopt,        // absolute_annual_rainfall
                  kFakeSoilCondition,  // optimal_soil_condition
                  kFakeSoilCondition,  // absolute_soil_condition
                  std::nullopt,        // optimal_latitude
                  std::nullopt,        // absolute_latitude
                  kFakeLight,          // optimal_light_condition
                  kFakeLight,          // absolute_light_condition
                  std::nullopt,        // climate_zone
                  std::nullopt,        // photo_period
                  std::nullopt         // crop_cycle
        ) {}
  environment::Plant GeneratePlantInstance() const override {
    return Plant("Fake");
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
