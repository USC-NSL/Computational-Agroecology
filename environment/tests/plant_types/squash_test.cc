#include <string>

#include <gtest/gtest.h>

#include "plant_types/squash.h"

using namespace environment;
using namespace environment::plant_type;

const std::string kSquashTypeName = "Squash";

TEST(SquashTest, GlobalVariableTest) {
  const Squash& squash =
      *(reinterpret_cast<const Squash*>(plant_type_to_plant[kSquashTypeName]));

  EXPECT_EQ(kSquashTypeName, squash.type_name);
  EXPECT_EQ("-", squash.display_symbol);
  EXPECT_EQ(true, squash.cultivar);

  ASSERT_TRUE(squash.optimal_soil_depth);
  EXPECT_EQ(PlantType::kSoilDepthRangeDeep, *(squash.optimal_soil_depth));
  ASSERT_TRUE(squash.absolute_soil_depth);
  EXPECT_EQ(PlantType::kSoilDepthRangeShallow, *(squash.absolute_soil_depth));

  EXPECT_FALSE(squash.base_temperature);
  ASSERT_TRUE(squash.optimal_temperature);
  EXPECT_EQ(MinMaxTemperature(20, 30), *(squash.optimal_temperature));
  ASSERT_TRUE(squash.absolute_temperature);
  EXPECT_EQ(MinMaxTemperature(9, 38), *(squash.absolute_temperature));

  ASSERT_TRUE(squash.optimal_annual_rainfall);
  EXPECT_EQ(MinMaxRainfall(600, 1000), *(squash.optimal_annual_rainfall));
  ASSERT_TRUE(squash.absolute_annual_rainfall);
  EXPECT_EQ(MinMaxRainfall(450, 2700), *(squash.absolute_annual_rainfall));

  ASSERT_TRUE(squash.optimal_soil_condition.fertility);
  EXPECT_EQ(SoilCondition::SoilFertility::HIGH,
            *(squash.optimal_soil_condition.fertility));
  ASSERT_TRUE(squash.optimal_soil_condition.salinity);
  EXPECT_EQ(SoilCondition::kSoilSalinityLow,
            *(squash.optimal_soil_condition.salinity));
  ASSERT_TRUE(squash.optimal_soil_condition.pH);
  EXPECT_EQ(MinMaxPair<double>(5.5, 7.5), *(squash.optimal_soil_condition.pH));
  ASSERT_TRUE(squash.optimal_soil_condition.drainage);
  EXPECT_EQ(SoilCondition::SoilDrainage::WELL,
            *(squash.optimal_soil_condition.drainage));

  ASSERT_TRUE(squash.absolute_soil_condition.fertility);
  EXPECT_EQ(SoilCondition::SoilFertility::LOW,
            *(squash.absolute_soil_condition.fertility));
  ASSERT_TRUE(squash.absolute_soil_condition.salinity);
  EXPECT_EQ(SoilCondition::kSoilSalinityLow,
            *(squash.absolute_soil_condition.salinity));
  ASSERT_TRUE(squash.absolute_soil_condition.pH);
  EXPECT_EQ(MinMaxPair<double>(5.0, 8.5), *(squash.absolute_soil_condition.pH));
  ASSERT_TRUE(squash.absolute_soil_condition.drainage);
  EXPECT_EQ(SoilCondition::SoilDrainage::WELL,
            *(squash.absolute_soil_condition.drainage));

  ASSERT_TRUE(squash.optimal_latitude);
  EXPECT_EQ(MinMaxPair<double>(10, std::numeric_limits<double>::infinity()),
            *(squash.optimal_latitude));
  ASSERT_TRUE(squash.absolute_latitude);
  EXPECT_EQ(MinMaxPair<double>(20, 50), *(squash.absolute_latitude));

  ASSERT_TRUE(squash.optimal_light_condition.light_intensity);
  EXPECT_EQ(MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                              Light::kLightIntensityVeryBright),
            *(squash.optimal_light_condition.light_intensity));
  EXPECT_FALSE(squash.optimal_light_condition.angle);
  ASSERT_TRUE(squash.absolute_light_condition.light_intensity);
  EXPECT_EQ(
      MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                        Light::kLightIntensityCloudySkies),
      *(squash.absolute_light_condition.light_intensity));
  EXPECT_FALSE(squash.absolute_light_condition.angle);

  ASSERT_TRUE(squash.climate_zone);
  EXPECT_EQ(std::set<Climate::ZoneType>(
                {Climate::TropicalWetAndDry, Climate::TropicalWet,
                 Climate::SteppeOrSemiArid, Climate::SubtropicalHumid,
                 Climate::SubtropicalDrySummer, Climate::SubtropicalDryWinter,
                 Climate::TemperateOceanic, Climate::TemperateContinental,
                 Climate::TemperateWithHumidWinters,
                 Climate::TemperateWithDryWinters}),
            *(squash.climate_zone));

  ASSERT_TRUE(squash.photo_period);
  EXPECT_EQ(MinMaxPair<uint32_t>(0, 24), *(squash.photo_period));

  ASSERT_TRUE(squash.crop_cycle);
  EXPECT_EQ(MinMaxPair<uint32_t>(80, 140), *(squash.crop_cycle));
}

TEST(SquashTest, GenerateTest) {
  const Squash& squash =
      *(reinterpret_cast<const Squash*>(plant_type_to_plant[kSquashTypeName]));
  auto plant = squash.GeneratePlantInstance();

  EXPECT_EQ(kSquashTypeName, plant.type_name);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}