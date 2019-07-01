#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "environment.h"
#include "simulators/actions/crop.h"
#include "agent/agent.h"

// This is an experimental main
int main() {
  environment::Location location(100, 100, 200, 200);
  environment::Config config("place name", location);
  environment::Terrain terrain(3);

  environment::Environment env(config, std::chrono::system_clock::now(),
                               terrain);

  const std::string kCornTypeName = "Corn";

  //Declare agent
  std::string agent_name = "q_learning";
  environment::Environment *env_pointer = &env;
  agent::Agent q_learning(agent_name, env_pointer);
  auto action = q_learning.RandomAction();

  simulator::action::crop::Add add_crop(
      environment::Coordinate(0, 0),
      std::chrono::system_clock::now() + std::chrono::minutes(1),
      std::chrono::duration<int>(), kCornTypeName);
  env.ReceiveAction(&add_crop);
  simulator::action::crop::Add add_another_crop(
      environment::Coordinate(1, 1),
      std::chrono::system_clock::now() + std::chrono::minutes(1),
      std::chrono::duration<int>(), kCornTypeName);
  env.ReceiveAction(&add_another_crop);

  simulator::action::crop::Remove future_add_crop(
      environment::Coordinate(1, 1),
      std::chrono::system_clock::now() + std::chrono::hours(1),
      std::chrono::duration<int>());
  env.ReceiveAction(&future_add_crop);



  std::cout << env << std::endl;

  env.JumpDuration(std::chrono::hours(5));

  std::cout << env << std::endl;

  return 0;
}