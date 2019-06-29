#include <gtest/gtest.h>

#include "soil.h"

using namespace environment;

TEST(SoilTest, ConstructorTest) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);

  EXPECT_EQ(Soil::CLAY, soil.texture);
  EXPECT_EQ(6.0, soil.pH);
  EXPECT_EQ(1.0, soil.salinity);
  EXPECT_EQ(2.0, soil.organic_matter);
  EXPECT_EQ(3.0, soil.water_content);
}

TEST(SoilTest, OperatorTest) {
  Soil lhs(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);
  Soil rhs(Soil::CLAY, 6.0, 1.0, 2.0, 3.0);

  EXPECT_TRUE(lhs == rhs);

  rhs.pH = 7.0;
  EXPECT_FALSE(lhs == rhs);
}

TEST(SoilCondition, ConstructorTest) {
  SoilCondition soil_condition(SoilCondition::SoilFertility::MODERATE,
                               SoilCondition::kSoilSalinityMedium,
                               MinMaxPair<double>(7.0, 7.0),
                               SoilCondition::SoilDrainage::WELL);

  EXPECT_EQ(SoilCondition::SoilFertility::MODERATE,
            soil_condition.fertility.value());
  EXPECT_EQ(SoilCondition::kSoilSalinityMedium,
            soil_condition.salinity.value());
  EXPECT_EQ(MinMaxPair<double>(7.0, 7.0), soil_condition.pH.value());
  EXPECT_EQ(SoilCondition::SoilDrainage::WELL, soil_condition.drainage.value());
}

TEST(SoilCondition, OperatorTest) {
  SoilCondition lhs(SoilCondition::SoilFertility::MODERATE,
                    SoilCondition::kSoilSalinityMedium,
                    MinMaxPair<double>(7.0, 7.0),
                    SoilCondition::SoilDrainage::WELL);
  SoilCondition rhs(SoilCondition::SoilFertility::MODERATE,
                    SoilCondition::kSoilSalinityMedium,
                    MinMaxPair<double>(7.0, 7.0),
                    SoilCondition::SoilDrainage::WELL);

  EXPECT_TRUE(lhs == rhs);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
