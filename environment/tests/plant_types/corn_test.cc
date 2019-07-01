#include <string>

#include <gtest/gtest.h>

#include "plant_types/corn.h"

using namespace environment;
using namespace environment::plant_type;

const std::string kCornTypeName = "Corn";

TEST(CornTest, GlobalVariableTest) {
  const Corn& corn =
      *(reinterpret_cast<const Corn*>(plant_type_to_plant[kCornTypeName]));

  EXPECT_EQ(kCornTypeName, corn.type_name);
  EXPECT_EQ("🌽", corn.display_symbol);
  EXPECT_EQ(true, corn.cultivar);

  ASSERT_TRUE(corn.optimal_soil_depth);
  EXPECT_EQ(PlantType::kSoilDepthRangeMedium, *(corn.optimal_soil_depth));
  ASSERT_TRUE(corn.absolute_soil_depth);
  EXPECT_EQ(PlantType::kSoilDepthRangeShallow, *(corn.absolute_soil_depth));

  EXPECT_FALSE(corn.base_temperature);
  ASSERT_TRUE(corn.optimal_temperature);
  EXPECT_EQ(MinMaxTemperature(18, 33), *(corn.optimal_temperature));
  ASSERT_TRUE(corn.absolute_temperature);
  EXPECT_EQ(MinMaxTemperature(10, 47), *(corn.absolute_temperature));

  ASSERT_TRUE(corn.optimal_annual_rainfall);
  EXPECT_EQ(MinMaxRainfall(600, 1200), *(corn.optimal_annual_rainfall));
  ASSERT_TRUE(corn.absolute_annual_rainfall);
  EXPECT_EQ(MinMaxRainfall(400, 1800), *(corn.absolute_annual_rainfall));

  ASSERT_TRUE(corn.optimal_soil_condition.fertility);
  EXPECT_EQ(SoilCondition::SoilFertility::HIGH,
            *(corn.optimal_soil_condition.fertility));
  ASSERT_TRUE(corn.optimal_soil_condition.salinity);
  EXPECT_EQ(SoilCondition::kSoilSalinityLow,
            *(corn.optimal_soil_condition.salinity));
  ASSERT_TRUE(corn.optimal_soil_condition.pH);
  EXPECT_EQ(MinMaxPair<double>(5.0, 7.0), *(corn.optimal_soil_condition.pH));
  EXPECT_FALSE(corn.optimal_soil_condition.drainage);

  ASSERT_TRUE(corn.absolute_soil_condition.fertility);
  EXPECT_EQ(SoilCondition::SoilFertility::LOW,
            *(corn.absolute_soil_condition.fertility));
  ASSERT_TRUE(corn.absolute_soil_condition.salinity);
  EXPECT_EQ(SoilCondition::kSoilSalinityMedium,
            *(corn.absolute_soil_condition.salinity));
  ASSERT_TRUE(corn.absolute_soil_condition.pH);
  EXPECT_EQ(MinMaxPair<double>(4.5, 8.5), *(corn.absolute_soil_condition.pH));
  EXPECT_FALSE(corn.absolute_soil_condition.drainage);

  EXPECT_FALSE(corn.optimal_latitude);
  ASSERT_TRUE(corn.absolute_latitude);
  EXPECT_EQ(MinMaxPair<double>(40, 48), *(corn.absolute_latitude));

  ASSERT_TRUE(corn.optimal_light_condition.light_intensity);
  EXPECT_EQ(MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                              Light::kLightIntensityVeryBright),
            *(corn.optimal_light_condition.light_intensity));
  EXPECT_FALSE(corn.optimal_light_condition.angle);
  ASSERT_TRUE(corn.absolute_light_condition.light_intensity);
  EXPECT_EQ(MinMaxPair<Light::LightIntensity>(Light::kLightIntensityClearSkies,
                                              Light::kLightIntensityVeryBright),
            *(corn.absolute_light_condition.light_intensity));
  EXPECT_FALSE(corn.absolute_light_condition.angle);

  EXPECT_FALSE(corn.climate_zone);

  EXPECT_FALSE(corn.photo_period);

  ASSERT_TRUE(corn.crop_cycle);
  EXPECT_EQ(MinMaxPair<uint32_t>(65, 365), *(corn.crop_cycle));
}

TEST(CornTest, GenerateTest) {
  const Corn& corn =
      *(reinterpret_cast<const Corn*>(plant_type_to_plant[kCornTypeName]));
  auto plant = corn.GeneratePlantInstance();

  EXPECT_EQ(kCornTypeName, plant.type_name);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}