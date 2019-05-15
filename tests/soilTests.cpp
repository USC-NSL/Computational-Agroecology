// Written 5/15/19 by Rastaar Haghi

#include <gtest/gtest.h>
#include <iostream>
#include "vector"
#include "../common/soil.h"



// SIMPLY ADDING WATER TO DATA MEMBER
TEST(Soil, ADDING_WATER)
{
	// create variable of data type soil with texture of silt, pH=7, salinity=3.7, orgMat=5, water=3
   Soil soil;
   soil.addWater(0.5);
   EXPECT_EQ(soil.water_content(), 0.53);
}

// WATER LEVELS SHOULD NOT GO NEGATIVE IF DRIED OUT
TEST(Soil, NEGATIVE_WATER)
{
	Soil soil;
	soil.addWater(-100.0);
	EXPECT_EQ(soil.water_content(), 0);
}

// NUTRIENT LEVELS SHOULDNT GO BELOW 0
TEST(Soil, NEGATIVE_NUTRIENTS)
{
	Soil soil;
	soil.addNitrogen(-5.99);
	EXPECT_EQ(soil.nutrients()[0], 0);

	soil.addPhosphorus(-10.99);
	EXPECT_EQ(soil.nutrients()[1], 0);

	soil.addPotassium(-8.99);
	EXPECT_EQ(soil.nutrients()[2], 0);

	soil.addCalcium(-4.99);
	EXPECT_EQ(soil.nutrients()[3], 0);

	soil.addMagnesium(-5.99);
	EXPECT_EQ(soil.nutrients()[4], 0);

	soil.addSulfur(-3.99);
	EXPECT_EQ(soil.nutrients()[5], 0);
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}