#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_MESSAGE_CONVERTOR_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_MESSAGE_CONVERTOR_H_

#include <chrono>

#include "environment/climate.h"
#include "environment/config.h"
#include "environment/environment.h"
#include "environment/location.h"
#include "environment/soil.h"
#include "environment/terrain.h"
#include "environment/weather.h"
#include "proto/environment.pb.h"

// time convertor
std::chrono::system_clock::time_point FromProtobuf(
    const int64_t timestamp_epoch_count);
int64_t ToProtobuf(const std::chrono::system_clock::time_point& timestamp);

// location convertor
environment::Location FromProtobuf(
    const data_format::Location& protobuf_location);
data_format::Location ToProtobuf(const environment::Location& location);

// config convertor
environment::Config FromProtobuf(const data_format::Config& protobuf_config);
data_format::Config ToProtobuf(const environment::Config& config);

// plant convertor
environment::Plant* FromProtobuf(const data_format::Plant& protobuf_plant);
data_format::Plant ToProtobuf(const environment::Plant& plant);

// soil convertor
environment::Soil FromProtobuf(const data_format::Soil& protobuf_soil);
data_format::Soil ToProtobuf(const environment::Soil& soil);

// cell convertor
environment::Cell FromProtobuf(const data_format::Terrain_Cell& protobuf_cell);
data_format::Terrain_Cell ToProtobuf(const environment::Cell& cell);

// terrain convertor
environment::Terrain FromProtobuf(const data_format::Terrain& protobuf_terrain);
data_format::Terrain ToProtobuf(const environment::Terrain& terrain);

// climate convertor
environment::Climate FromProtobuf(const data_format::Climate& climate_protobuf);
data_format::Climate ToProtobuf(const environment::Climate& climate);

// weather convertor
environment::Weather FromProtobuf(const data_format::Weather& weather_protobuf);
data_format::Weather ToProtobuf(const environment::Weather& weather);

// environment convertor
data_format::Environment ToProtobuf(
    const environment::Environment& environment);

#endif