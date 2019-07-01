#include <string>

#include <gtest/gtest.h>

#include "plant_types/plant_type.h"
#include "utility.h"

using namespace environment;
using namespace environment::plant_type;

TEST(PlantTypeTest, Initialization) {
  SoilCondition optimal_soil_condition(
      SoilCondition::SoilFertility::HIGH, SoilCondition::kSoilSalinityLow,
      MinMaxPair<double>(5.5, 7.0), SoilCondition::SoilDrainage::WELL);
  SoilCondition absolute_soil_condition(
      SoilCondition::SoilFertility::MODERATE, SoilCondition::kSoilSalinityLow,
      MinMaxPair<double>(4.5, 8.7), SoilCondition::SoilDrainage::WELL);

  Light optimal_light_condition(
      MinMaxPair<Light::LightIntensity>(Light::kLightIntensityClearSkies,
                                        Light::kLightIntensityClearSkies),
      std::nullopt);
  Light absolute_light_condition(
      MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                        Light::kLightIntensityCloudySkies),
      std::nullopt);

  PlantType plant_type(
      "Abelmoschus esculentus",           // type_name
      "x",                                // display_symbol
      true,                               // cultivar
      PlantType::kSoilDepthRangeShallow,  // optimal_soil_depth
      PlantType::kSoilDepthRangeShallow,  // absolute_soil_depth
      std::nullopt,                       // base_temperature
      MinMaxTemperature(20, 30),          // optimal_temperature
      MinMaxTemperature(12, 35),          // absolute_temperature
      MinMaxRainfall(60, 1200),           // optimal_annual_rainfall
      MinMaxRainfall(300, 2500),          // absolute_annual_rainfall
      optimal_soil_condition,             // optimal_soil_condition
      absolute_soil_condition,            // absolute_soil_condition
      std::nullopt,                       // optimal_latitude
      std::nullopt,                       // absolute_latitude
      optimal_light_condition,            // optimal_light_condition
      absolute_light_condition,           // absolute_light_condition
      std::set({Climate::TropicalWet, Climate::SteppeOrSemiArid,
                Climate::SubtropicalHumid, Climate::SubtropicalDrySummer,
                Climate::SubtropicalDryWinter}),  // climate_zone
      PhotoPeriod(0, 12),                         // photo_period
      MinMaxPair<uint32_t>(50, 180)               // crop_cycle
  );

  EXPECT_EQ("Abelmoschus esculentus", plant_type.type_name);
  EXPECT_EQ("x", plant_type.display_symbol);
  EXPECT_EQ(true, plant_type.cultivar);

  ASSERT_TRUE(plant_type.optimal_soil_depth);
  EXPECT_EQ(PlantType::kSoilDepthRangeShallow,
            plant_type.optimal_soil_depth.value());
  ASSERT_TRUE(plant_type.absolute_soil_depth);
  EXPECT_EQ(PlantType::kSoilDepthRangeShallow,
            plant_type.absolute_soil_depth.value());

  EXPECT_FALSE(plant_type.base_temperature);
  ASSERT_TRUE(plant_type.optimal_temperature);
  EXPECT_EQ(MinMaxTemperature(20, 30), plant_type.optimal_temperature.value());
  ASSERT_TRUE(plant_type.absolute_temperature);
  EXPECT_EQ(MinMaxTemperature(12, 35), plant_type.absolute_temperature.value());

  ASSERT_TRUE(plant_type.optimal_annual_rainfall);
  EXPECT_EQ(MinMaxRainfall(60, 1200),
            plant_type.optimal_annual_rainfall.value());
  ASSERT_TRUE(plant_type.absolute_annual_rainfall);
  EXPECT_EQ(MinMaxRainfall(300, 2500),
            plant_type.absolute_annual_rainfall.value());

  EXPECT_EQ(optimal_soil_condition, plant_type.optimal_soil_condition);
  EXPECT_EQ(absolute_soil_condition, plant_type.absolute_soil_condition);

  EXPECT_FALSE(plant_type.optimal_latitude);
  EXPECT_FALSE(plant_type.absolute_latitude);

  EXPECT_EQ(optimal_light_condition, plant_type.optimal_light_condition);
  EXPECT_EQ(absolute_light_condition, plant_type.absolute_light_condition);

  ASSERT_TRUE(plant_type.climate_zone);
  EXPECT_EQ(std::set({Climate::TropicalWet, Climate::SteppeOrSemiArid,
                      Climate::SubtropicalHumid, Climate::SubtropicalDrySummer,
                      Climate::SubtropicalDryWinter}),
            plant_type.climate_zone.value());

  ASSERT_TRUE(plant_type.photo_period);
  EXPECT_EQ(PhotoPeriod(0, 12), plant_type.photo_period.value());

  ASSERT_TRUE(plant_type.crop_cycle);
  EXPECT_EQ(MinMaxPair<uint32_t>(50, 180), plant_type.crop_cycle.value());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}