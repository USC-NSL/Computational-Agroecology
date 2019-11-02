#include <string>

#include <gtest/gtest.h>

#include "environment/plant_builder.h"

using namespace config;
using namespace environment;

const std::string kBeanTypeName = "bean";

TEST(BeanTest, GenerateTest) {
  Config dumb_config("place name", Location(100, 101, 201, 200));
  Climate dumb_climate(dumb_config);
  Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  Meteorology dumb_meteorology(std::chrono::system_clock::now(),
                               dumb_config.location, dumb_climate.climate_zone,
                               dumb_weather);
  std::unique_ptr<environment::Plant> plant(
      environment::PlantBuilder::NewPlant(kBeanTypeName, dumb_meteorology));
  EXPECT_EQ(kBeanTypeName, plant->name());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}