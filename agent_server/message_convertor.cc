#include "message_convertor.h"

std::chrono::system_clock::time_point FromProtobuf(
    const int64_t timestamp_epoch_count) {
  auto epoch_count = std::chrono::nanoseconds(timestamp_epoch_count);
  return std::chrono::system_clock::time_point(epoch_count);
}

int64_t ToProtobuf(const std::chrono::system_clock::time_point& timestamp) {
  return timestamp.time_since_epoch().count();
}

environment::Location FromProtobuf(
    const data_format::Location& protobuf_location) {
  return environment::Location(
      protobuf_location.longitude_left(), protobuf_location.longitude_right(),
      protobuf_location.latitude_top(), protobuf_location.latitude_bottom());
}

data_format::Location ToProtobuf(const environment::Location& location) {
  data_format::Location location_protobuf;

  location_protobuf.set_longitude_left(location.longitude_left);
  location_protobuf.set_longitude_right(location.longitude_right);
  location_protobuf.set_latitude_top(location.latitude_top);
  location_protobuf.set_latitude_bottom(location.latitude_bottom);

  return location_protobuf;
}

environment::Config FromProtobuf(const data_format::Config& protobuf_config) {
  environment::Location location = FromProtobuf(protobuf_config.location());
  return environment::Config(protobuf_config.name(), location);
}

data_format::Config ToProtobuf(const environment::Config& config) {
  data_format::Config config_protobuf;

  config_protobuf.set_name(config.name);
  *(config_protobuf.mutable_location()) = ToProtobuf(config.location);

  return config_protobuf;
}

environment::Plant FromProtobuf(const data_format::Plant& protobuf_plant) {
  environment::Plant plant(protobuf_plant.type_name());

  plant.health = protobuf_plant.health();
  plant.flowering = protobuf_plant.flowering();
  plant.accumulated_gdd = protobuf_plant.accumulated_gdd();

  switch (protobuf_plant.maturity()) {
    case data_format::Plant_Maturity_SEED:
      plant.maturity = environment::Plant::SEED;
      break;
    case data_format::Plant_Maturity_SEEDLING:
      plant.maturity = environment::Plant::SEEDLING;
      break;
    case data_format::Plant_Maturity_JUVENILE:
      plant.maturity = environment::Plant::JUVENILE;
      break;
    case data_format::Plant_Maturity_MATURE:
      plant.maturity = environment::Plant::MATURE;
      break;
    case data_format::Plant_Maturity_OLD:
      plant.maturity = environment::Plant::OLD;
      break;
  }

  plant.base_temperature = protobuf_plant.base_temperature();
  plant.gdd_thresholds = std::vector<int>(protobuf_plant.gdd_thresholds().begin(), protobuf_plant.gdd_thresholds().end());

  return plant;
}

data_format::Plant ToProtobuf(const environment::Plant& plant) {
  data_format::Plant plant_protobuf;

  plant_protobuf.set_type_name(plant.type_name);
  plant_protobuf.set_health(plant.health);
  plant_protobuf.set_flowering(plant.flowering);
  plant_protobuf.set_accumulated_gdd(plant.accumulated_gdd);

  switch (plant.maturity) {
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

  plant_protobuf.set_base_temperature(plant.base_temperature);
  *(plant_protobuf.mutable_gdd_thresholds()) = {plant.gdd_thresholds.begin(), plant.gdd_thresholds.end()};

  return plant_protobuf;
}

environment::Soil FromProtobuf(const data_format::Soil& protobuf_soil) {
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

  return environment::Soil(
      soil_texture, protobuf_soil.ph(), protobuf_soil.salinity(),
      protobuf_soil.organic_matter(), protobuf_soil.water_content());
}

data_format::Soil ToProtobuf(const environment::Soil& soil) {
  data_format::Soil soil_protobuf;
  data_format::Soil_Texture soil_texture;
  switch (soil.texture) {
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
  soil_protobuf.set_ph(soil.pH);
  soil_protobuf.set_salinity(soil.salinity);
  soil_protobuf.set_organic_matter(soil.organic_matter);
  soil_protobuf.set_water_content(soil.water_content);

  return soil_protobuf;
}

environment::Cell FromProtobuf(const data_format::Terrain_Cell& protobuf_cell) {
  environment::Soil soil = FromProtobuf(protobuf_cell.soil());
  environment::Cell cell(protobuf_cell.size(), soil);
  if (protobuf_cell.has_plant()) {
    cell.plant = FromProtobuf(protobuf_cell.plant());
  } else {
    cell.plant = std::nullopt;
  }

  return cell;
}

data_format::Terrain_Cell ToProtobuf(const environment::Cell& cell) {
  data_format::Terrain_Cell cell_protobuf;

  cell_protobuf.set_size(cell.size);
  *(cell_protobuf.mutable_soil()) = ToProtobuf(cell.soil);
  if (cell.plant.has_value()) {
    *(cell_protobuf.mutable_plant()) = ToProtobuf(*(cell.plant));
  }

  return cell_protobuf;
}

environment::Terrain FromProtobuf(
    const data_format::Terrain& protobuf_terrain) {
  int size = protobuf_terrain.tiles_size();
  environment::Terrain terrain(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      terrain.tiles()[i][j] = FromProtobuf(protobuf_terrain.tiles(i).cells(j));
    }
  }

  return terrain;
}

data_format::Terrain ToProtobuf(const environment::Terrain& terrain) {
  data_format::Terrain terrain_protobuf;

  for (size_t i = 0; i < terrain.width(); ++i) {
    auto row = terrain_protobuf.add_tiles();
    for (size_t j = 0; j < terrain.length(); ++j) {
      *(row->add_cells()) = ToProtobuf(terrain.tiles()[i][j]);
    }
  }

  return terrain_protobuf;
}

environment::Climate FromProtobuf(
    const data_format::Climate& climate_protobuf) {
  double temp_max = climate_protobuf.yearly_temperature().max();
  double temp_min = climate_protobuf.yearly_temperature().min();
  double rain_max = climate_protobuf.yearly_rainfall().max();
  double rain_min = climate_protobuf.yearly_rainfall().min();
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

  return environment::Climate(temp_max, temp_min, rain_max, rain_min,
                              zone_type);
}

data_format::Climate ToProtobuf(const environment::Climate& climate) {
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

  climate_protobuf.mutable_yearly_temperature()->set_max(
      climate.yearly_temperature.max);
  climate_protobuf.mutable_yearly_temperature()->set_min(
      climate.yearly_temperature.min);
  climate_protobuf.mutable_yearly_rainfall()->set_max(
      climate.yearly_rainfall.max);
  climate_protobuf.mutable_yearly_rainfall()->set_min(
      climate.yearly_rainfall.min);

  return climate_protobuf;
}

environment::Weather FromProtobuf(
    const data_format::Weather& weather_protobuf) {
  double temp_max = weather_protobuf.temperature().max();
  double temp_min = weather_protobuf.temperature().min();
  double rainfall = weather_protobuf.rainfall();

  return environment::Weather(temp_max, temp_min, rainfall);
}

data_format::Weather ToProtobuf(const environment::Weather& weather) {
  data_format::Weather weather_protobuf;

  weather_protobuf.mutable_temperature()->set_max(weather.temperature.max);
  weather_protobuf.mutable_temperature()->set_min(weather.temperature.min);
  weather_protobuf.set_rainfall(weather.rainfall);

  return weather_protobuf;
}

data_format::Environment ToProtobuf(
    const environment::Environment& environment) {
  data_format::Environment env_protobuf;

  *(env_protobuf.mutable_config()) = ToProtobuf(environment.config());
  *(env_protobuf.mutable_climate()) = ToProtobuf(environment.climate());
  env_protobuf.set_timestamp_epoch_count(ToProtobuf(environment.timestamp()));
  *(env_protobuf.mutable_terrain()) = ToProtobuf(environment.terrain());
  *(env_protobuf.mutable_weather()) = ToProtobuf(environment.weather());

  return env_protobuf;
}