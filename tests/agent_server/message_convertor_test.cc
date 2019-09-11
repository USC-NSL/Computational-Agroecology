#include <chrono>
#include <iostream>

#include <gtest/gtest.h>

#include "agent/actions/crop.h"
#include "agent/resource.h"
#include "agent_server/message_convertor.h"
#include "environment/config.h"
#include "environment/location.h"
#include "environment/plant_builder.h"
#include "environment/terrain.h"

using namespace environment;

TEST(MessageConvertorTest, TimePointConvertorTest) {
  auto tp = std::chrono::system_clock::now();

  auto tp_protobuf = ToProtobuf(tp);
  EXPECT_EQ(tp, FromProtobufTimePoint(tp_protobuf));
}

TEST(MessageConvertorTest, DurationConvertorTest) {
  auto dur = std::chrono::hours(1);

  auto dur_protobuf = ToProtobuf(dur);
  EXPECT_EQ(dur, FromProtobufDuration(dur_protobuf));
}

TEST(MessageConvertorTest, LocationConvertorTest) {
  Location location(100.0, 200.0, 300.0, 400.0);

  auto location_protobuf = ToProtobuf(location);
  EXPECT_EQ(location, FromProtobuf(location_protobuf));
}

TEST(MessageConvertorTest, ConfigConvertorTest) {
  Config config("place name", Location(100.0, 200.0, 300.0, 400.0));

  auto config_protobuf = ToProtobuf(config);
  EXPECT_EQ(config, FromProtobuf(config_protobuf));
}

#define TEST_PLANT_CONVERTOR(_PLANT, _PLANT_PROTOBUF)                         \
  EXPECT_EQ((_PLANT).name(), (_PLANT_PROTOBUF).name());                       \
  EXPECT_EQ((_PLANT).trunk_size(), (_PLANT_PROTOBUF).trunk_size());           \
  EXPECT_EQ((_PLANT).root_size(), (_PLANT_PROTOBUF).root_size());             \
  EXPECT_EQ((_PLANT).health(), (_PLANT_PROTOBUF).health());                   \
  EXPECT_EQ((_PLANT).flowering(), (_PLANT_PROTOBUF).flowering());             \
  EXPECT_EQ((_PLANT).height(), (_PLANT_PROTOBUF).height());                   \
  EXPECT_EQ((_PLANT).accumulated_gdd(), (_PLANT_PROTOBUF).accumulated_gdd()); \
  switch ((_PLANT).maturity()) {                                              \
    case environment::Plant::SEED:                                            \
      EXPECT_EQ(data_format::Plant_Maturity_SEED,                             \
                (_PLANT_PROTOBUF).maturity());                                \
      break;                                                                  \
    case environment::Plant::SEEDLING:                                        \
      EXPECT_EQ(data_format::Plant_Maturity_SEEDLING,                         \
                (_PLANT_PROTOBUF).maturity());                                \
      break;                                                                  \
    case environment::Plant::JUVENILE:                                        \
      EXPECT_EQ(data_format::Plant_Maturity_JUVENILE,                         \
                (_PLANT_PROTOBUF).maturity());                                \
      break;                                                                  \
    case environment::Plant::MATURE:                                          \
      EXPECT_EQ(data_format::Plant_Maturity_MATURE,                           \
                (_PLANT_PROTOBUF).maturity());                                \
      break;                                                                  \
    case environment::Plant::OLD:                                             \
      EXPECT_EQ(data_format::Plant_Maturity_OLD,                              \
                (_PLANT_PROTOBUF).maturity());                                \
      break;                                                                  \
  }                                                                           \
  EXPECT_EQ((_PLANT).produce(), (_PLANT_PROTOBUF).produce());                 \
  EXPECT_EQ(                                                                  \
      (_PLANT).params().at(environment::PlantProperty::GDD_BASE_TEMPERATURE), \
      (_PLANT_PROTOBUF).params().gdd_base_temperature());                     \
  EXPECT_EQ((_PLANT).params().at(                                             \
                environment::PlantProperty::MIN_ABSOLUTE_TEMPERATURE),        \
            (_PLANT_PROTOBUF).params().min_absolute_temperature());           \
  EXPECT_EQ((_PLANT).params().at(                                             \
                environment::PlantProperty::MAX_ABSOLUTE_TEMPERATURE),        \
            (_PLANT_PROTOBUF).params().max_absolute_temperature());           \
  EXPECT_EQ((_PLANT).params().at(                                             \
                environment::PlantProperty::MIN_NEW_GROWTH_TEMPERATURE),      \
            (_PLANT_PROTOBUF).params().min_new_growth_temperature());         \
  EXPECT_EQ((_PLANT).params().at(                                             \
                environment::PlantProperty::MAX_NEW_GROWTH_TEMPERATURE),      \
            (_PLANT_PROTOBUF).params().max_new_growth_temperature());         \
  EXPECT_EQ(                                                                  \
      (_PLANT).params().at(environment::PlantProperty::MIN_PHOTO_PERIOD),     \
      (_PLANT_PROTOBUF).params().min_photo_period());                         \
  EXPECT_EQ(                                                                  \
      (_PLANT).params().at(environment::PlantProperty::MAX_PHOTO_PERIOD),     \
      (_PLANT_PROTOBUF).params().max_photo_period());                         \
  EXPECT_EQ(                                                                  \
      (_PLANT).params().at(environment::PlantProperty::MAX_HARVEST_YIELD),    \
      (_PLANT_PROTOBUF).params().max_harvest_yield());                        \
  EXPECT_EQ((_PLANT).params().at(                                             \
                environment::PlantProperty::GDD_UNITS_AFTER_FULL_BLOOM),      \
            (_PLANT_PROTOBUF).params().gdd_units_after_full_bloom())

TEST(MessageConvertorTest, PlantConvertorTest) {
  Plant* plant = environment::PlantBuilder::NewPlant("bean");

  ASSERT_NE(nullptr, plant);

  auto plant_protobuf = ToProtobuf(*plant);

  TEST_PLANT_CONVERTOR(*plant, plant_protobuf);
}

TEST(MessageConvertorTest, SoilConvertorTest) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);

  auto soil_protobuf = ToProtobuf(soil);
  EXPECT_EQ(soil, FromProtobuf(soil_protobuf));
}

TEST(MessageConvertorTest, CoordinateConvertorTest) {
  Coordinate coordinate(5, 10, 15);

  auto coordinate_protobuf = ToProtobuf(coordinate);
  EXPECT_EQ(coordinate, FromProtobuf(coordinate_protobuf));
}

TEST(MessageConvertorTest, TerrainConvertorTest) {
  Terrain terrain(5);

  auto terrain_protobuf = ToProtobuf(terrain);

  EXPECT_EQ(terrain.yield(), terrain_protobuf.yield());
  EXPECT_EQ(terrain.size(), terrain_protobuf.size());

  for (size_t i = 0; i < terrain.GetAllPlants().size(); ++i) {
    EXPECT_EQ(terrain.GetAllPlants()[i]->position(),
              FromProtobuf(terrain_protobuf.plants()[i].position()));
    TEST_PLANT_CONVERTOR(*(terrain.GetAllPlants()[i]),
                         terrain_protobuf.plants()[i].plant());
  }

  for (size_t i = 0; i < terrain.length(); ++i) {
    for (size_t j = 0; j < terrain.width(); ++j) {
      size_t idx = i * terrain.length() + j;
      const environment::Coordinate& pos =
          FromProtobuf(terrain_protobuf.soil()[idx].position());
      EXPECT_EQ(*(terrain.GetSoil(pos)),
                FromProtobuf(terrain_protobuf.soil()[idx].soil()));
    }
  }
}

TEST(MessageConvertorTest, ClimateConvertorTest) {
  Climate climate(1.0, 2.0, 3.0, 4.0, Climate::Polar);

  auto climate_protobuf = ToProtobuf(climate);
  EXPECT_EQ(climate, FromProtobuf(climate_protobuf));
}

TEST(MessageConvertorTest, WeatherConvertorTest) {
  Weather weather(1, 2, 3, 4, 5, 6);

  auto weather_protobuf = ToProtobuf(weather);
  EXPECT_EQ(weather, FromProtobuf(weather_protobuf));
}

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

TEST_F(ActionConvertorTest, ActionAddCropTest) {
  agent::action::crop::Add action(applied_range, start_time_step, duration,
                                  cost, "Corn");

  auto action_protobuf = ToProtobuf(action);
  EXPECT_EQ(action, FromProtobuf(action_protobuf));
}

TEST_F(ActionConvertorTest, ActionRemoveCropTest) {
  agent::action::crop::Remove action(applied_range, start_time_step, duration,
                                     cost);

  auto action_protobuf = ToProtobuf(action);
  EXPECT_EQ(action, FromProtobuf(action_protobuf));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
