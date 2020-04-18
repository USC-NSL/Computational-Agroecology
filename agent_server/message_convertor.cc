#include "message_convertor.h"

std::chrono::system_clock::time_point FromProtobufTimePoint(
    const int64_t timestamp_epoch_count) {
  auto epoch_count = std::chrono::microseconds(timestamp_epoch_count);
  //changed from nano to micro
  return std::chrono::system_clock::time_point(epoch_count);
}

int64_t ToProtobuf(const std::chrono::system_clock::time_point &timestamp) {
  return timestamp.time_since_epoch().count();
}

std::chrono::duration<int> FromProtobufDuration(
    const int64_t time_step_epoch_count) {
  return std::chrono::seconds(time_step_epoch_count);
}
int64_t ToProtobuf(const std::chrono::duration<int> &time_step_length) {
  return time_step_length.count();
}

config::Location FromProtobuf(const data_format::Location &protobuf_location) {
  return config::Location(
      protobuf_location.longitude_left(), protobuf_location.longitude_right(),
      protobuf_location.latitude_top(), protobuf_location.latitude_bottom());
}

data_format::Location ToProtobuf(const config::Location &location) {
  data_format::Location location_protobuf;

  location_protobuf.set_longitude_left(location.longitude_left);
  location_protobuf.set_longitude_right(location.longitude_right);
  location_protobuf.set_latitude_top(location.latitude_top);
  location_protobuf.set_latitude_bottom(location.latitude_bottom);

  return location_protobuf;
}

config::Config FromProtobuf(const data_format::Config &protobuf_config) {
  config::Location location = FromProtobuf(protobuf_config.location());
  return config::Config(protobuf_config.name(), location);
}

data_format::Config ToProtobuf(const config::Config &config) {
  data_format::Config config_protobuf;

  config_protobuf.set_name(config.name);
  *(config_protobuf.mutable_location()) = ToProtobuf(config.location);

  return config_protobuf;
}

data_format::Plant ToProtobuf(const environment::Plant &plant) {
  data_format::Plant plant_protobuf;

  plant_protobuf.set_name(plant.name());
  plant_protobuf.set_trunk_size(plant.trunk_size());
  plant_protobuf.set_root_size(plant.root_size());
  plant_protobuf.set_health(plant.health());
  plant_protobuf.set_flowering(plant.flowering());
  plant_protobuf.set_height(plant.height());
  plant_protobuf.set_accumulated_gdd(plant.accumulated_gdd());

  switch (plant.maturity()) {
    case environment::Plant::SEED:
      plant_protobuf.set_maturity(data_format::Plant_Maturity_SEED);
      break;
    case environment::Plant::SEEDLING:
      plant_protobuf.set_maturity(data_format::Plant_Maturity_SEEDLING);
      break;
    case environment::Plant::JUVENILE:
      plant_protobuf.set_maturity(data_format::Plant_Maturity_JUVENILE);
      break;
    case environment::Plant::MATURE:
      plant_protobuf.set_maturity(data_format::Plant_Maturity_MATURE);
      break;
    case environment::Plant::OLD:
      plant_protobuf.set_maturity(data_format::Plant_Maturity_OLD);
      break;
  }

  plant_protobuf.set_produce(plant.produce());

  plant_protobuf.mutable_params()->set_gdd_base_temperature(
      plant.params().at(environment::PlantProperty::GDD_BASE_TEMPERATURE));
  plant_protobuf.mutable_params()->set_min_absolute_temperature(
      plant.params().at(environment::PlantProperty::MIN_ABSOLUTE_TEMPERATURE));
  plant_protobuf.mutable_params()->set_max_absolute_temperature(
      plant.params().at(environment::PlantProperty::MAX_ABSOLUTE_TEMPERATURE));
  plant_protobuf.mutable_params()->set_min_new_growth_temperature(
      plant.params().at(
          environment::PlantProperty::MIN_NEW_GROWTH_TEMPERATURE));
  plant_protobuf.mutable_params()->set_max_new_growth_temperature(
      plant.params().at(
          environment::PlantProperty::MAX_NEW_GROWTH_TEMPERATURE));
  plant_protobuf.mutable_params()->set_min_photo_period(
      plant.params().at(environment::PlantProperty::MIN_PHOTO_PERIOD));
  plant_protobuf.mutable_params()->set_max_photo_period(
      plant.params().at(environment::PlantProperty::MAX_PHOTO_PERIOD));
  plant_protobuf.mutable_params()->set_max_harvest_yield(
      plant.params().at(environment::PlantProperty::MAX_HARVEST_YIELD));
  plant_protobuf.mutable_params()->set_gdd_units_after_full_bloom(
      plant.params().at(
          environment::PlantProperty::GDD_UNITS_AFTER_FULL_BLOOM));

  return plant_protobuf;
}

environment::Soil FromProtobuf(const data_format::Soil &protobuf_soil) {
  environment::Soil::Texture soil_texture;
  switch (protobuf_soil.texture()) {
    case data_format::Soil_Texture::Soil_Texture_CLAY:
      soil_texture = environment::Soil::CLAY;
      break;
    case data_format::Soil_Texture::Soil_Texture_SAND:
      soil_texture = environment::Soil::SAND;
      break;
    case data_format::Soil_Texture::Soil_Texture_SILT:
      soil_texture = environment::Soil::SILT;
      break;
  }

  return environment::Soil(soil_texture, protobuf_soil.ph(),
                           protobuf_soil.salinity(),
                           protobuf_soil.organic_matter(),
                           protobuf_soil.water_content().water_amount_1(),
                           protobuf_soil.water_content().water_amount_2());
}

data_format::Soil ToProtobuf(const environment::Soil &soil) {
  data_format::Soil soil_protobuf;
  data_format::Soil_Texture soil_texture;
  switch (soil.texture()) {
    case environment::Soil::CLAY:
      soil_texture = data_format::Soil_Texture::Soil_Texture_CLAY;
      break;
    case environment::Soil::SAND:
      soil_texture = data_format::Soil_Texture::Soil_Texture_SAND;
      break;
    case environment::Soil::SILT:
      soil_texture = data_format::Soil_Texture::Soil_Texture_SILT;
      break;
  }

  soil_protobuf.set_texture(soil_texture);
  soil_protobuf.set_ph(soil.pH());
  soil_protobuf.set_salinity(soil.salinity());
  soil_protobuf.set_organic_matter(soil.organic_matter());
  soil_protobuf.mutable_water_content()->set_water_amount_1(
      soil.water_content().water_amount_1);
  soil_protobuf.mutable_water_content()->set_water_amount_2(
      soil.water_content().water_amount_2);

  return soil_protobuf;
}

environment::Coordinate FromProtobuf(
    const data_format::Coordinate &protobuf_coordinate) {
  return environment::Coordinate(protobuf_coordinate.x(),
                                 protobuf_coordinate.y(),
                                 protobuf_coordinate.z());
}

data_format::Coordinate ToProtobuf(const environment::Coordinate &coordinate) {
  data_format::Coordinate coordinate_protobuf;
  coordinate_protobuf.set_x(coordinate.x);
  coordinate_protobuf.set_y(coordinate.y);
  coordinate_protobuf.set_z(coordinate.z);

  return coordinate_protobuf;
}

data_format::Terrain ToProtobuf(const environment::Terrain &terrain) {
  data_format::Terrain terrain_protobuf;

  terrain_protobuf.set_yield(terrain.yield());
  terrain_protobuf.set_size(terrain.size());

  for (const auto &p : terrain.plant_container()) {
    auto *new_plant = terrain_protobuf.add_plants();
    *(new_plant->mutable_position()) = ToProtobuf(p->position());
    *(new_plant->mutable_plant()) = ToProtobuf(*p);
  }

  for (size_t i = 0; i < terrain.length(); ++i) {
    for (size_t j = 0; j < terrain.width(); ++j) {
      environment::Coordinate pos(i, j);
      auto *new_soil = terrain_protobuf.add_soil();
      new_soil->mutable_position()->set_x(i);
      new_soil->mutable_position()->set_y(j);

      *(new_soil->mutable_soil()) = ToProtobuf(terrain.soil_container()[pos]);
    }
  }
  return terrain_protobuf;
}

environment::Climate FromProtobuf(
    const data_format::Climate &climate_protobuf) {
  double temp_min = climate_protobuf.yearly_temperature().min();
  double temp_max = climate_protobuf.yearly_temperature().max();
  double rain_min = climate_protobuf.yearly_rainfall().min();
  double rain_max = climate_protobuf.yearly_rainfall().max();
  environment::Climate::ZoneType zone_type;

  switch (climate_protobuf.climate_zone()) {
    case data_format::Climate_ZoneType_TropicalWetAndDry:
      zone_type = environment::Climate::TropicalWetAndDry;
      break;
    case data_format::Climate_ZoneType_TropicalWet:
      zone_type = environment::Climate::TropicalWet;
      break;
    case data_format::Climate_ZoneType_DesertOrArid:
      zone_type = environment::Climate::DesertOrArid;
      break;
    case data_format::Climate_ZoneType_SteppeOrSemiArid:
      zone_type = environment::Climate::SteppeOrSemiArid;
      break;
    case data_format::Climate_ZoneType_SubtropicalHumid:
      zone_type = environment::Climate::SubtropicalHumid;
      break;
    case data_format::Climate_ZoneType_SubtropicalDrySummer:
      zone_type = environment::Climate::SubtropicalDrySummer;
      break;
    case data_format::Climate_ZoneType_SubtropicalDryWinter:
      zone_type = environment::Climate::SubtropicalDryWinter;
      break;
    case data_format::Climate_ZoneType_TemperateOceanic:
      zone_type = environment::Climate::TemperateOceanic;
      break;
    case data_format::Climate_ZoneType_TemperateContinental:
      zone_type = environment::Climate::TemperateContinental;
      break;
    case data_format::Climate_ZoneType_TemperateWithHumidWinters:
      zone_type = environment::Climate::TemperateWithHumidWinters;
      break;
    case data_format::Climate_ZoneType_TemperateWithDryWinters:
      zone_type = environment::Climate::TemperateWithDryWinters;
      break;
    case data_format::Climate_ZoneType_Boreal:
      zone_type = environment::Climate::Boreal;
      break;
    case data_format::Climate_ZoneType_Polar:
      zone_type = environment::Climate::Polar;
      break;
  }

  return environment::Climate(temp_min, temp_max, rain_min, rain_max,
                              zone_type);
}

data_format::Climate ToProtobuf(const environment::Climate &climate) {
  data_format::Climate climate_protobuf;

  switch (climate.climate_zone) {
    case environment::Climate::TropicalWetAndDry:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_TropicalWetAndDry);
      break;
    case environment::Climate::TropicalWet:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_TropicalWet);
      break;
    case environment::Climate::DesertOrArid:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_DesertOrArid);
      break;
    case environment::Climate::SteppeOrSemiArid:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_SteppeOrSemiArid);
      break;
    case environment::Climate::SubtropicalHumid:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_SubtropicalHumid);
      break;
    case environment::Climate::SubtropicalDrySummer:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_SubtropicalDrySummer);
      break;
    case environment::Climate::SubtropicalDryWinter:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_SubtropicalDryWinter);
      break;
    case environment::Climate::TemperateOceanic:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_TemperateOceanic);
      break;
    case environment::Climate::TemperateContinental:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_TemperateContinental);
      break;
    case environment::Climate::TemperateWithHumidWinters:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_TemperateWithHumidWinters);
      break;
    case environment::Climate::TemperateWithDryWinters:
      climate_protobuf.set_climate_zone(
          data_format::Climate_ZoneType_TemperateWithDryWinters);
      break;
    case environment::Climate::Boreal:
      climate_protobuf.set_climate_zone(data_format::Climate_ZoneType_Boreal);
      break;
    case environment::Climate::Polar:
      climate_protobuf.set_climate_zone(data_format::Climate_ZoneType_Polar);
      break;
  }

  climate_protobuf.mutable_yearly_temperature()->set_min(
      climate.yearly_temperature.min);
  climate_protobuf.mutable_yearly_temperature()->set_max(
      climate.yearly_temperature.max);
  climate_protobuf.mutable_yearly_rainfall()->set_min(
      climate.yearly_rainfall.min);
  climate_protobuf.mutable_yearly_rainfall()->set_max(
      climate.yearly_rainfall.max);

  return climate_protobuf;
}

environment::Weather FromProtobuf(
    const data_format::Weather &weather_protobuf) {
  return environment::Weather(weather_protobuf.total_sunshine_hour(),
                              weather_protobuf.air_temperature().min(),
                              weather_protobuf.air_temperature().max(),
                              weather_protobuf.relative_humidity(),
                              weather_protobuf.wind_speed(),
                              weather_protobuf.rainfall());
}

data_format::Weather ToProtobuf(const environment::Weather &weather) {
  data_format::Weather weather_protobuf;

  weather_protobuf.set_total_sunshine_hour(weather.total_sunshine_hour);
  weather_protobuf.mutable_air_temperature()->set_min(
      weather.air_temperature.min);
  weather_protobuf.mutable_air_temperature()->set_max(
      weather.air_temperature.max);
  weather_protobuf.set_relative_humidity(weather.relative_humidity);
  weather_protobuf.set_wind_speed(weather.wind_speed);
  weather_protobuf.set_rainfall(weather.rainfall);

  return weather_protobuf;
}

data_format::Environment ToProtobuf(
    const environment::Environment &environment) {
  data_format::Environment env_protobuf;

  *(env_protobuf.mutable_config()) = ToProtobuf(environment.config());
  *(env_protobuf.mutable_climate()) = ToProtobuf(environment.climate());
  env_protobuf.set_timestamp_epoch_count(ToProtobuf(environment.timestamp()));
  *(env_protobuf.mutable_terrain()) = ToProtobuf(environment.terrain());
  *(env_protobuf.mutable_weather()) = ToProtobuf(environment.weather());

  return env_protobuf;
}

void FromProtobuf(
    const agent_server::service::AgentActionConfig &config_protobuf,
    std::vector<environment::Coordinate> *applied_range,
    int64_t *start_time_step, int64_t *duration, agent::Resources *cost) {
  applied_range->clear();
  for (const auto &protobuf_coordinate : config_protobuf.applied_range()) {
    applied_range->push_back(FromProtobuf(protobuf_coordinate));
  }

  *start_time_step = config_protobuf.start_time_step();
  int64_t end_time = config_protobuf.end_time_step();
  *duration = end_time - *start_time_step;

  for (const auto &protobuf_cost : config_protobuf.cost()) {
    agent::ResourceType type;
    switch (protobuf_cost.resource_type()) {
      case (::agent_server::service::AgentActionConfig_Cost_ResourceType_MONEY):
        type = agent::ResourceType::MONEY;
        break;
      case (::agent_server::service::AgentActionConfig_Cost_ResourceType_LABOR):
        type = agent::ResourceType::LABOR;
        break;
    }
    (*cost)[type] = protobuf_cost.count();
  }
}

agent_server::service::AgentActionConfig ToProtobuf(
    const std::vector<environment::Coordinate> &applied_range,
    const int64_t &start_time_step, const int64_t &duration,
    const agent::Resources &cost) {
  agent_server::service::AgentActionConfig agent_action_config;

  for (const auto &coordinate : applied_range) {
    *(agent_action_config.add_applied_range()) = ToProtobuf(coordinate);
  }

  agent_action_config.set_start_time_step(start_time_step);
  agent_action_config.set_end_time_step(start_time_step + duration);

  for (const auto &c : cost) {
    auto cost_ptr = agent_action_config.add_cost();
    ::agent_server::service::AgentActionConfig_Cost_ResourceType type;
    switch (c.first) {
      case agent::ResourceType::MONEY:
        type =
            ::agent_server::service::AgentActionConfig_Cost_ResourceType_MONEY;
        break;
      case agent::ResourceType::LABOR:
        type =
            ::agent_server::service::AgentActionConfig_Cost_ResourceType_LABOR;
        break;
    }

    cost_ptr->set_resource_type(type);
    cost_ptr->set_count(c.second);
  }

  return agent_action_config;
}

agent::action::crop::Add FromProtobuf(
    const agent_server::service::AgentAddCropRequest &add_crop_protobuf) {
  std::vector<environment::Coordinate> applied_range;
  int64_t start_time_step;
  int64_t duration;
  agent::Resources cost;

  FromProtobuf(add_crop_protobuf.action_config(), &applied_range,
               &start_time_step, &duration, &cost);

  return agent::action::crop::Add(applied_range, start_time_step, duration,
                                  cost, add_crop_protobuf.crop_type_name());
}

agent_server::service::AgentAddCropRequest ToProtobuf(
    const agent::action::crop::Add &action) {
  agent_server::service::AgentAddCropRequest agent_add_crop_request;

  *(agent_add_crop_request.mutable_action_config()) =
      ToProtobuf(action.applied_range(), action.start_time_step(),
                 action.duration(), action.cost());

  agent_add_crop_request.set_crop_type_name(action.crop_type_name());

  return agent_add_crop_request;
}

agent::action::crop::Remove FromProtobuf(
    const agent_server::service::AgentRemoveCropRequest &remove_crop_protobuf) {
  std::vector<environment::Coordinate> applied_range;
  int64_t start_time_step;
  int64_t duration;
  agent::Resources cost;

  FromProtobuf(remove_crop_protobuf.action_config(), &applied_range,
               &start_time_step, &duration, &cost);

  return agent::action::crop::Remove(applied_range, start_time_step, duration,
                                     cost);
}

agent_server::service::AgentRemoveCropRequest ToProtobuf(
    const agent::action::crop::Remove &action) {
  agent_server::service::AgentRemoveCropRequest agent_remove_crop_request;

  *(agent_remove_crop_request.mutable_action_config()) =
      ToProtobuf(action.applied_range(), action.start_time_step(),
                 action.duration(), action.cost());

  return agent_remove_crop_request;
}