#include <chrono>
#include <iostream>

#include <gtest/gtest.h>

#include "agent/actions/crop.h"
#include "agent/resource.h"
#include "agent_server/message_convertor.h"
#include "config/config.h"
#include "config/location.h"
#include "environment/plant_builder.h"
#include "environment/terrain.h"

using namespace config;
using namespace environment;

// All unit tests below are testing on converting selected data structures (in
// C++) in our system into the format in protobuf.
//
// For the full list of protobuf definitions, you may refer to
// /agent_server/proto/environment.proto

// A helper function to test if a plant object in C++ is equal to the one in
// protobuf.
void TestPlantConvertor(const Plant &plant,
                        const data_format::Plant &plant_protobuf) {
  EXPECT_EQ(plant.name(), plant_protobuf.name());
  EXPECT_EQ(plant.trunk_size(), plant_protobuf.trunk_size());
  EXPECT_EQ(plant.root_size(), plant_protobuf.root_size());
  EXPECT_EQ(plant.health(), plant_protobuf.health());
  EXPECT_EQ(plant.flowering(), plant_protobuf.flowering());
  EXPECT_EQ(plant.height(), plant_protobuf.height());
  EXPECT_EQ(plant.accumulated_gdd(), plant_protobuf.accumulated_gdd());
  switch (plant.maturity()) {
    case environment::Plant::SEED:
      EXPECT_EQ(data_format::Plant_Maturity_SEED, plant_protobuf.maturity());
      break;
    case environment::Plant::SEEDLING:
      EXPECT_EQ(data_format::Plant_Maturity_SEEDLING,
                plant_protobuf.maturity());
      break;
    case environment::Plant::JUVENILE:
      EXPECT_EQ(data_format::Plant_Maturity_JUVENILE,
                plant_protobuf.maturity());
      break;
    case environment::Plant::MATURE:
      EXPECT_EQ(data_format::Plant_Maturity_MATURE, plant_protobuf.maturity());
      break;
    case environment::Plant::OLD:
      EXPECT_EQ(data_format::Plant_Maturity_OLD, plant_protobuf.maturity());
      break;
  }
  EXPECT_EQ(plant.produce(), plant_protobuf.produce());
  EXPECT_EQ(plant.params().at(environment::PlantProperty::GDD_BASE_TEMPERATURE),
            plant_protobuf.params().gdd_base_temperature());
  EXPECT_EQ(
      plant.params().at(environment::PlantProperty::MIN_ABSOLUTE_TEMPERATURE),
      plant_protobuf.params().min_absolute_temperature());
  EXPECT_EQ(
      plant.params().at(environment::PlantProperty::MAX_ABSOLUTE_TEMPERATURE),
      plant_protobuf.params().max_absolute_temperature());
  EXPECT_EQ(
      plant.params().at(environment::PlantProperty::MIN_NEW_GROWTH_TEMPERATURE),
      plant_protobuf.params().min_new_growth_temperature());
  EXPECT_EQ(
      plant.params().at(environment::PlantProperty::MAX_NEW_GROWTH_TEMPERATURE),
      plant_protobuf.params().max_new_growth_temperature());
  EXPECT_EQ(plant.params().at(environment::PlantProperty::MIN_PHOTO_PERIOD),
            plant_protobuf.params().min_photo_period());
  EXPECT_EQ(plant.params().at(environment::PlantProperty::MAX_PHOTO_PERIOD),
            plant_protobuf.params().max_photo_period());
  EXPECT_EQ(plant.params().at(environment::PlantProperty::MAX_HARVEST_YIELD),
            plant_protobuf.params().max_harvest_yield());
  EXPECT_EQ(
      plant.params().at(environment::PlantProperty::GDD_UNITS_AFTER_FULL_BLOOM),
      plant_protobuf.params().gdd_units_after_full_bloom());
}

// This tests on converting a `std::chrono::system_clock::time_point` to and
// from protobuf.
TEST(MessageConvertorTest, TimePointConvertorTest) {
  auto tp = std::chrono::system_clock::now();

  auto tp_protobuf = ToProtobuf(tp);
  EXPECT_EQ(tp, FromProtobufTimePoint(tp_protobuf));
}

// This tests on converting a `std::chrono::duration<int>` to and from protobuf.
TEST(MessageConvertorTest, DurationConvertorTest) {
  auto dur = std::chrono::hours(1);

  auto dur_protobuf = ToProtobuf(dur);
  EXPECT_EQ(dur, FromProtobufDuration(dur_protobuf));
}

// This tests on converting a `struct environment::Location` to and from
// protobuf.
TEST(MessageConvertorTest, LocationConvertorTest) {
  Location location(100.0, 200.0, 300.0, 400.0);

  auto location_protobuf = ToProtobuf(location);
  EXPECT_EQ(location, FromProtobuf(location_protobuf));
}

// This tests on converting a `struct environment::Config` to and from protobuf.
TEST(MessageConvertorTest, ConfigConvertorTest) {
  Config config("place name", Location(100.0, 200.0, 300.0, 400.0));

  auto config_protobuf = ToProtobuf(config);
  EXPECT_EQ(config, FromProtobuf(config_protobuf));
}

// This tests on converting a `class environment::Plant` to protobuf.
TEST(MessageConvertorTest, PlantConvertorTest) {
  Config dumb_config("place name", Location(100, 101, 201, 200));
  Climate dumb_climate(dumb_config);
  Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  Meteorology dumb_meteorology(std::chrono::system_clock::now(),
                               dumb_config.location, dumb_climate.climate_zone,
                               dumb_weather);
  Plant *plant = environment::PlantBuilder::NewPlant("bean", dumb_meteorology);

  ASSERT_NE(nullptr, plant);

  auto plant_protobuf = ToProtobuf(*plant);

  TestPlantConvertor(*plant, plant_protobuf);
}

// This tests on converting a `struct environment::Soil` to and from protobuf.
TEST(MessageConvertorTest, SoilConvertorTest) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 0.0, 0.0);

  auto soil_protobuf = ToProtobuf(soil);
  EXPECT_EQ(soil, FromProtobuf(soil_protobuf));
}

// This tests on converting a `struct environment::Coordinate` to and from
// protobuf.
TEST(MessageConvertorTest, CoordinateConvertorTest) {
  Coordinate coordinate(5, 10, 15);

  auto coordinate_protobuf = ToProtobuf(coordinate);
  EXPECT_EQ(coordinate, FromProtobuf(coordinate_protobuf));
}

// This tests on converting a `class environment::Terrain` to protobuf.
TEST(MessageConvertorTest, TerrainConvertorTest) {
  Config dumb_config("place name", Location(100, 101, 201, 200));
  Climate dumb_climate(dumb_config);
  Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  Meteorology dumb_meteorology(std::chrono::system_clock::now(),
                               dumb_config.location, dumb_climate.climate_zone,
                               dumb_weather);
  TerrainRawData dumb_terrain_raw_data(5, 0);
  Terrain terrain(dumb_terrain_raw_data, dumb_meteorology);

  auto terrain_protobuf = ToProtobuf(terrain);

  EXPECT_EQ(terrain.yield(), terrain_protobuf.yield());
  EXPECT_EQ(terrain.size(), terrain_protobuf.size());

  {
    size_t i = 0;
    for (const auto &plant : terrain.plant_container()) {
      EXPECT_EQ(plant->position(),
                FromProtobuf(terrain_protobuf.plants()[i].position()));
      TestPlantConvertor(*plant, terrain_protobuf.plants()[i].plant());
      ++i;
    }
  }

  for (size_t i = 0; i < terrain.length(); ++i) {
    for (size_t j = 0; j < terrain.width(); ++j) {
      size_t idx = i * terrain.length() + j;
      const environment::Coordinate &pos =
          FromProtobuf(terrain_protobuf.soil()[idx].position());
      EXPECT_EQ(terrain.soil_container()[pos],
                FromProtobuf(terrain_protobuf.soil()[idx].soil()));
    }
  }
}

// This tests on converting a `struct environment::Climate` to and from
// protobuf.
TEST(MessageConvertorTest, ClimateConvertorTest) {
  Climate climate(1.0, 2.0, 3.0, 4.0, Climate::Polar);

  auto climate_protobuf = ToProtobuf(climate);
  EXPECT_EQ(climate, FromProtobuf(climate_protobuf));
}

// This tests on converting a `struct environment::Config` to and from protobuf.
TEST(MessageConvertorTest, WeatherConvertorTest) {
  Weather weather(1, 2, 3, 4, 5, 6);

  auto weather_protobuf = ToProtobuf(weather);
  EXPECT_EQ(weather, FromProtobuf(weather_protobuf));
}

// This prepares some parameters which will be used by action constructors.
class ActionConvertorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (size_t i = 0; i < 5; ++i) {
      applied_range.push_back(environment::Coordinate(i, i + 1));
    }

    start_time_step = 0;
    duration = 1;

    cost[agent::ResourceType::MONEY] = 10;
    cost[agent::ResourceType::LABOR] = 20;
  }

  std::vector<environment::Coordinate> applied_range;
  int64_t start_time_step;
  int64_t duration;
  agent::Resources cost;
};

// This tests on converting a `class agent::action::crop::Add` to and from
// protobuf.
TEST_F(ActionConvertorTest, ActionAddCropTest) {
  agent::action::crop::Add action(applied_range, start_time_step, duration,
                                  cost, "Corn");

  auto action_protobuf = ToProtobuf(action);
  EXPECT_EQ(action, FromProtobuf(action_protobuf));
}

// This tests on converting a `class agent::action::crop::Remove` to and from
// protobuf.
TEST_F(ActionConvertorTest, ActionRemoveCropTest) {
  agent::action::crop::Remove action(applied_range, start_time_step, duration,
                                     cost);

  auto action_protobuf = ToProtobuf(action);
  EXPECT_EQ(action, FromProtobuf(action_protobuf));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
