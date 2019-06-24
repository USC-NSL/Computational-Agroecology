#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "environment.h"
#include "simulators/actions/crop.h"

// This is an experimental main
int main() {
  environment::Location location(100, 100, 200, 200);
  environment::Config config("place name", location);
  environment::Terrain terrain(5);

  environment::Environment env(config, std::chrono::system_clock::now(),
                               terrain);

  const std::string kCornTypeName = "Corn";
  simulator::action::crop::Add add_crop(
      environment::Coordinate(0, 0),
      std::chrono::system_clock::now() + std::chrono::minutes(10),
      std::chrono::duration<int>(), kCornTypeName);
  env.ReceiveAction(&add_crop);
  simulator::action::crop::Add add_another_crop(
      environment::Coordinate(1, 1),
      std::chrono::system_clock::now() + std::chrono::minutes(20),
      std::chrono::duration<int>(), kCornTypeName);
  env.ReceiveAction(&add_another_crop);

  simulator::action::crop::Add future_add_crop(
      environment::Coordinate(2, 2),
      std::chrono::system_clock::now() + std::chrono::hours(48),
      std::chrono::duration<int>(), kCornTypeName);
  env.ReceiveAction(&future_add_crop);

  std::cout << env << std::endl;

  env.JumpDuration(std::chrono::hours(24));

  std::cout << env << std::endl;

  return 0;
}