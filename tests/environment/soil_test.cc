#include <gtest/gtest.h>

#include "environment/soil.h"

using namespace environment;

// Soil is initialized correctly from parameters
TEST(SoilTest, ConstructorTest) {
  Soil soil(Soil::CLAY, 6.0, 1.0, 2.0, 0.0, 0.0);

  EXPECT_EQ(Soil::CLAY, soil.texture());
  EXPECT_EQ(6.0, soil.pH());
  EXPECT_EQ(1.0, soil.salinity());
  EXPECT_EQ(2.0, soil.organic_matter());
  EXPECT_EQ(0.0, soil.water_content().water_amount_1);
  EXPECT_EQ(0.0, soil.water_content().water_amount_2);
}

// Soil equivalence test
TEST(SoilTest, OperatorTest) {
  Soil lhs(Soil::CLAY, 6.0, 1.0, 2.0, 0.0, 0.0);
  Soil rhs(Soil::CLAY, 6.0, 1.0, 2.0, 0.0, 0.0);

  EXPECT_TRUE(lhs == rhs);

  Soil rhs_not_equal(Soil::CLAY, 7.0, 1.0, 2.0, 0.0, 0.0);
  EXPECT_FALSE(lhs == rhs_not_equal);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
