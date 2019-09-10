#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_MESSAGE_CONVERTOR_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_MESSAGE_CONVERTOR_H_

#include <chrono>

#include "agent/q_learning.h"
#include "agent/resource.h"
#include "environment/climate.h"
#include "environment/config.h"
#include "environment/coordinate.h"
#include "environment/environment.h"
#include "environment/location.h"
#include "environment/plant.h"
#include "environment/soil.h"
#include "environment/terrain.h"
#include "environment/weather.h"
#include "proto/agent_server.pb.h"
#include "proto/environment.pb.h"

// time convertor
std::chrono::system_clock::time_point FromProtobufTimePoint(
    const int64_t timestamp_epoch_count);
int64_t ToProtobuf(const std::chrono::system_clock::time_point& timestamp);
std::chrono::duration<int> FromProtobufDuration(
    const int64_t time_step_epoch_count);
int64_t ToProtobuf(const std::chrono::duration<int>& time_step_length);

// location convertor
environment::Location FromProtobuf(
    const data_format::Location& protobuf_location);
data_format::Location ToProtobuf(const environment::Location& location);

// config convertor
environment::Config FromProtobuf(const data_format::Config& protobuf_config);
data_format::Config ToProtobuf(const environment::Config& config);

// plant convertor
data_format::Plant ToProtobuf(const environment::Plant& plant);

// soil convertor
environment::Soil FromProtobuf(const data_format::Soil& protobuf_soil);
data_format::Soil ToProtobuf(const environment::Soil& soil);

// coordinate convertor
environment::Coordinate FromProtobuf(
    const data_format::Coordinate& protobuf_coordinate);
data_format::Coordinate ToProtobuf(const environment::Coordinate& coordinate);

// terrain convertor
data_format::Terrain ToProtobuf(const environment::Terrain& terrain);

// climate convertor
environment::Climate FromProtobuf(const data_format::Climate& climate_protobuf);
data_format::Climate ToProtobuf(const environment::Climate& climate);

// weather convertor
data_format::Weather ToProtobuf(const environment::Weather& weather);

// environment convertor
data_format::Environment ToProtobuf(
    const environment::Environment& environment);

// action convertor
void FromProtobuf(
    const agent_server::service::AgentActionConfig& config_protobuf,
    std::vector<environment::Coordinate>* applied_range,
    int64_t* start_time_step, int64_t* duration, agent::Resources* cost);
agent_server::service::AgentActionConfig ToProtobuf(
    const std::vector<environment::Coordinate>& applied_range,
    const int64_t& start_time_step, const int64_t& duration,
    const agent::Resources& cost);
agent::action::crop::Add FromProtobuf(
    const agent_server::service::AgentAddCropRequest& add_crop_protobuf);
agent_server::service::AgentAddCropRequest ToProtobuf(
    const agent::action::crop::Add& action);
agent::action::crop::Remove FromProtobuf(
    const agent_server::service::AgentRemoveCropRequest& remove_crop_protobuf);
agent_server::service::AgentRemoveCropRequest ToProtobuf(
    const agent::action::crop::Remove& action);

#endif