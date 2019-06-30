#include <string>

#include <gtest/gtest.h>

#include "plant_types/bean.h"

using namespace environment;
using namespace environment::plant_type;

const std::string kBeanTypeName = "Bean";

TEST(BeanTest, GlobalVariableTest) {
  const Bean& bean =
      *(reinterpret_cast<const Bean*>(plant_type_to_plant[kBeanTypeName]));

  EXPECT_EQ(kBeanTypeName, bean.type_name);
  EXPECT_EQ("o", bean.display_symbol);
  EXPECT_EQ(true, bean.cultivar);

  ASSERT_TRUE(bean.optimal_soil_depth);
  EXPECT_EQ(PlantType::kSoilDepthRangeMedium, *(bean.optimal_soil_depth));
  ASSERT_TRUE(bean.absolute_soil_depth);
  EXPECT_EQ(PlantType::kSoilDepthRangeShallow, *(bean.absolute_soil_depth));

  EXPECT_FALSE(bean.base_temperature);
  ASSERT_TRUE(bean.optimal_temperature);
  EXPECT_EQ(MinMaxTemperature(18, 28), *(bean.optimal_temperature));
  ASSERT_TRUE(bean.absolute_temperature);
  EXPECT_EQ(MinMaxTemperature(5, 32), *(bean.absolute_temperature));

  ASSERT_TRUE(bean.optimal_annual_rainfall);
  EXPECT_EQ(MinMaxRainfall(650, 1000), *(bean.optimal_annual_rainfall));
  ASSERT_TRUE(bean.absolute_annual_rainfall);
  EXPECT_EQ(MinMaxRainfall(250, 2600), *(bean.absolute_annual_rainfall));

  ASSERT_TRUE(bean.optimal_soil_condition.fertility);
  EXPECT_EQ(SoilCondition::SoilFertility::HIGH,
            *(bean.optimal_soil_condition.fertility));
  ASSERT_TRUE(bean.optimal_soil_condition.salinity);
  EXPECT_EQ(SoilCondition::kSoilSalinityLow,
            *(bean.optimal_soil_condition.salinity));
  ASSERT_TRUE(bean.optimal_soil_condition.pH);
  EXPECT_EQ(MinMaxPair<double>(6.0, 7.0), *(bean.optimal_soil_condition.pH));
  ASSERT_TRUE(bean.optimal_soil_condition.drainage);
  EXPECT_EQ(SoilCondition::SoilDrainage::WELL,
            *(bean.optimal_soil_condition.drainage));

  ASSERT_TRUE(bean.absolute_soil_condition.fertility);
  EXPECT_EQ(SoilCondition::SoilFertility::MODERATE,
            *(bean.absolute_soil_condition.fertility));
  ASSERT_TRUE(bean.absolute_soil_condition.salinity);
  EXPECT_EQ(SoilCondition::kSoilSalinityLow,
            *(bean.absolute_soil_condition.salinity));
  ASSERT_TRUE(bean.absolute_soil_condition.pH);
  EXPECT_EQ(MinMaxPair<double>(4.5, 8.6), *(bean.absolute_soil_condition.pH));
  ASSERT_TRUE(bean.absolute_soil_condition.drainage);
  EXPECT_EQ(SoilCondition::SoilDrainage::WELL,
            *(bean.absolute_soil_condition.drainage));

  ASSERT_TRUE(bean.optimal_latitude);
  EXPECT_EQ(MinMaxPair<double>(30, std::numeric_limits<double>::infinity()),
            *(bean.optimal_latitude));
  ASSERT_TRUE(bean.absolute_latitude);
  EXPECT_EQ(MinMaxPair<double>(50, 60), *(bean.absolute_latitude));

  ASSERT_TRUE(bean.optimal_light_condition.light_intensity);
  EXPECT_EQ(MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                              Light::kLightIntensityClearSkies),
            *(bean.optimal_light_condition.light_intensity));
  EXPECT_FALSE(bean.optimal_light_condition.angle);
  ASSERT_TRUE(bean.absolute_light_condition.light_intensity);
  EXPECT_EQ(
      MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                        Light::kLightIntensityCloudySkies),
      *(bean.absolute_light_condition.light_intensity));
  EXPECT_FALSE(bean.absolute_light_condition.angle);

  ASSERT_TRUE(bean.climate_zone);
  EXPECT_EQ(std::set<Climate::ZoneType>(
                {Climate::TropicalWetAndDry, Climate::TropicalWet,
                 Climate::SteppeOrSemiArid, Climate::SubtropicalHumid,
                 Climate::SubtropicalDrySummer, Climate::SubtropicalDryWinter,
                 Climate::TemperateOceanic, Climate::TemperateContinental,
                 Climate::TemperateWithHumidWinters,
                 Climate::TemperateWithDryWinters}),
            *(bean.climate_zone));

  ASSERT_TRUE(bean.photo_period);
  EXPECT_EQ(MinMaxPair<uint32_t>(0, 24), *(bean.photo_period));

  ASSERT_TRUE(bean.crop_cycle);
  EXPECT_EQ(MinMaxPair<uint32_t>(100, 150), *(bean.crop_cycle));
}

TEST(BeanTest, GenerateTest) {
  const Bean& bean =
      *(reinterpret_cast<const Bean*>(plant_type_to_plant[kBeanTypeName]));
  auto plant = bean.GeneratePlantInstance();

  EXPECT_EQ(kBeanTypeName, plant.type_name);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}