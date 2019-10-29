#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "agent/actions/crop.h"
#include "agent/agent.h"
#include "agent/q_learning.h"
#include "environment/environment.h"

// This is an experimental main
int main() {
  config::Location location(100, 101, 201, 200);
  config::Config config("place name", location);
  config::TerrainRawData terrain_raw_data(3);
  // environment::Terrain terrain(3);

  environment::Environment env(config, terrain_raw_data, std::chrono::system_clock::now(),
                               std::chrono::hours(1));

  const std::string kBeanTypeName = "bean";

  // Declare agent
  std::string agent_name = "q_learning";
  environment::Environment *env_pointer = &env;
  // Declare Qlearning
  agent::Qlearning agent_test(agent_name, env_pointer, 10, 54);
  // Create Action
  agent::ActionID action = {
      agent_test.RandomInt(0, env.terrain().size() - 1),
      agent_test.RandomInt(0, env.terrain().size() - 1),
      ::agent::action::ActionType(agent_test.RandomInt(
          0, ::agent::action::ActionType::NUM_ACTIONS - 1)),
      1,
      0,
      kBeanTypeName};
  auto action_obj = agent_test.CreateAction(action);

  // ApplyRandomAction function on envionment
  agent_test.ApplyRandomAction(0, 3);

  agent::action::crop::Add add_crop(environment::Coordinate(0, 0), 1, 0,
                                    kBeanTypeName);
  env.ReceiveAction(&add_crop);
  agent::action::crop::Add add_another_crop(environment::Coordinate(1, 1), 2, 0,
                                            kBeanTypeName);
  env.ReceiveAction(&add_another_crop);

  agent::action::crop::Add future_add_crop(environment::Coordinate(2, 2), 48, 0,
                                           kBeanTypeName);
  env.ReceiveAction(&future_add_crop);

  std::cout << env << std::endl;

  env.JumpForwardTimeStep(24);

  std::cout << env << std::endl;

  return 0;
}
