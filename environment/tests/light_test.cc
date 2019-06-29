#include <gtest/gtest.h>

#include "light.h"

using namespace environment;

TEST(LightTest, ConstructorTest) {
  Light light(
      MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                        Light::kLightIntensityClearSkies),
      1.1);

  EXPECT_EQ(Light::kLightIntensityVeryBright, light.light_intensity->min);
  EXPECT_EQ(Light::kLightIntensityClearSkies, light.light_intensity->max);
  EXPECT_EQ(1.1, light.angle);
}

TEST(LightTest, OperatorTest) {
  Light lhs(MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                              Light::kLightIntensityClearSkies),
            1.1);
  Light rhs(MinMaxPair<Light::LightIntensity>(Light::kLightIntensityVeryBright,
                                              Light::kLightIntensityClearSkies),
            1.1);

  EXPECT_TRUE(lhs == rhs);
}

TEST(LightIntensityTest, ConstructorTest) {
  Light::LightIntensity light_intensity(Light::CLOUDY_SKIES, 0.1, 0.5);

  EXPECT_EQ(Light::CLOUDY_SKIES, light_intensity.category);
  EXPECT_EQ(0.1, light_intensity.indirect_light);
  EXPECT_EQ(0.5, light_intensity.light_lost);
}

TEST(LightIntensityTest, OperatorTest) {
  Light::LightIntensity lhs(Light::CLOUDY_SKIES, 0.1, 0.5);
  Light::LightIntensity rhs(Light::CLOUDY_SKIES, 0.1, 0.5);

  EXPECT_TRUE(lhs == rhs);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
