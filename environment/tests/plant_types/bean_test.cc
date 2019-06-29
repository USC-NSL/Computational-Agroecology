#include <string>

#include <gtest/gtest.h>

#include "plant_types/bean.h"

using namespace environment::plant_type;

const std::string kBeanTypeName = "Bean";

TEST(BeanTest, GlobalVariableTest) {
  const Bean& bean =
      *(reinterpret_cast<const Bean*>(plant_type_to_plant[kBeanTypeName]));

  EXPECT_EQ(kBeanTypeName, bean.type_name);
  EXPECT_EQ("o", bean.display_symbol);
  EXPECT_EQ(true, bean.cultivar);

  EXPECT_FALSE(bean.optimal_soil_depth);
  EXPECT_FALSE(bean.absolute_soil_depth);

  EXPECT_FALSE(bean.base_temperature);
  EXPECT_FALSE(bean.optimal_temperature);
  EXPECT_FALSE(bean.absolute_temperature);

  EXPECT_FALSE(bean.optimal_annual_rainfall);
  EXPECT_FALSE(bean.absolute_annual_rainfall);

  EXPECT_FALSE(bean.optimal_soil_condition.fertility);
  EXPECT_FALSE(bean.optimal_soil_condition.salinity);
  EXPECT_FALSE(bean.optimal_soil_condition.pH);
  EXPECT_FALSE(bean.optimal_soil_condition.drainage);
  EXPECT_FALSE(bean.absolute_soil_condition.fertility);
  EXPECT_FALSE(bean.absolute_soil_condition.salinity);
  EXPECT_FALSE(bean.absolute_soil_condition.pH);
  EXPECT_FALSE(bean.absolute_soil_condition.drainage);

  EXPECT_FALSE(bean.optimal_latitude);
  EXPECT_FALSE(bean.absolute_latitude);

  EXPECT_FALSE(bean.optimal_light_condition.light_intensity);
  EXPECT_FALSE(bean.optimal_light_condition.angle);
  EXPECT_FALSE(bean.absolute_light_condition.light_intensity);
  EXPECT_FALSE(bean.absolute_light_condition.angle);

  EXPECT_FALSE(bean.climate_zone);

  EXPECT_FALSE(bean.photo_period);

  EXPECT_FALSE(bean.crop_cycle);
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