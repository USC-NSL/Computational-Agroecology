#include <chrono>
#include <iostream>

#include <gtest/gtest.h>

#include "environment/config.h"
#include "environment/location.h"
#include "environment/terrain.h"
#include "message_convertor.h"

using namespace environment;

TEST(MessageConvertorTest, TimeConvertorTest) {
  auto tp = std::chrono::system_clock::now();

  auto tp_protobuf = ToProtobuf(tp);
  EXPECT_EQ(tp, FromProtobuf(tp_protobuf));
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

TEST(MessageConvertorTest, PlantConvertorTest) {
  Plant plant("Corn");

  auto plant_protobuf = ToProtobuf(plant);
  EXPECT_EQ(plant, FromProtobuf(plant_protobuf));
}

TEST(MessageConvertorTest, SoilConvertorTest) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);

  auto soil_protobuf = ToProtobuf(soil);
  EXPECT_EQ(soil, FromProtobuf(soil_protobuf));
}

TEST(MessageConvertorTest, CellConvertorTest) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);
  Cell cell(soil);

  auto cell_protobuf = ToProtobuf(cell);
  EXPECT_EQ(cell, FromProtobuf(cell_protobuf));
}

TEST(MessageConvertorTest, TerrainConvertorTest) {
  Terrain terrain(5);

  auto terrain_protobuf = ToProtobuf(terrain);
  auto terrain_from_protobuf = FromProtobuf(terrain_protobuf);
  EXPECT_EQ(terrain.tiles(), terrain_from_protobuf.tiles());
}

TEST(MessageConvertorTest, ClimateConvertorTest) {
  Climate climate(1.0, 2.0, 3.0, 4.0, Climate::Polar);

  auto climate_protobuf = ToProtobuf(climate);
  EXPECT_EQ(climate, FromProtobuf(climate_protobuf));
}

TEST(MessageConvertorTest, WeatherConvertorTest) {
  Climate climate(1.0, 2.0, 3.0, 4.0, Climate::Polar);
  Weather weather(climate, std::chrono::system_clock::now());

  auto weather_protobuf = ToProtobuf(weather);
  EXPECT_EQ(weather, FromProtobuf(weather_protobuf));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
