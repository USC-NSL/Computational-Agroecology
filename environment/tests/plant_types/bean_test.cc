

#include <gtest/gtest.h>

#include "plant_types/bean.h"

using namespace environment::plant_type;

TEST(BeanTest, GlobalVariableTest) {
  const Bean& bean =
      *(reinterpret_cast<const Bean*>(plant_type_to_plant[TypeId::BEAN]));

  EXPECT_EQ(TypeId::BEAN, bean.id);
  EXPECT_EQ("bean", bean.name);
  EXPECT_EQ("o", bean.display_symbol);
  EXPECT_EQ(true, bean.cultivar);
  EXPECT_EQ(0.0, bean.base_temperature);
  EXPECT_EQ(MaxMinTemperature(0, 0), bean.optimal_temperature);
  EXPECT_EQ(MaxMinTemperature(0, 0), bean.absolute_temperature);
  EXPECT_EQ(MaxMinRainfall(0, 0), bean.optimal_annual_rainfall);
  EXPECT_EQ(MaxMinRainfall(0, 0), bean.absolute_annual_rainfall);
}

TEST(BeanTest, GenerateTest) {
  const Bean& bean =
      *(reinterpret_cast<const Bean*>(plant_type_to_plant[TypeId::BEAN]));
  auto plant = bean.GeneratePlantInstance();

  EXPECT_EQ(TypeId::BEAN, plant->type);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}