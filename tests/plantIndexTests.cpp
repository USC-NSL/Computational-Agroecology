#include <gtest/gtest.h>
#include <iostream>
#include "vector"
#include "../common/plant_index.cc"



// SIMPLY ADDING WATER TO DATA MEMBER
TEST(CALC_GDD, BASIC_CALC)
{
	PlantIndex temp;
	temp.setBaseTemp(50);
    EXPECT_EQ(temp.CalcGDD(0, 100), 0);
}

// TESTS FOR THE CHECK_NEEDS FUNCTION
TEST(CHECKNEEDS, BASIC_CALC)
{
	PlantIndex temp;
	temp.getGDDThresholds()[0] = 32;
	temp.getGDDThresholds()[1] = 100;
	temp.getGDDThresholds()[2] = 5;
	temp.getGDDThresholds()[3] = 10;
	EXPECT_EQ(temp.CheckNeeds(7, 33, 99), true);

	EXPECT_EQ(temp.CheckNeeds(4, 33, 99), false);

	EXPECT_EQ(temp.CheckNeeds(7, 31, 99), false);

	EXPECT_EQ(temp.CheckNeeds(7, 33, 101), false);

	EXPECT_EQ(temp.CheckNeeds(7, 50, 50), true);
}

// TEST THE TRANSITION FUNCTION OF plant_index.cc
TEST(TRANSITION, BASIC_CALC)
{
	PlantIndex temp;
	temp.getGDDThresholds()[0] = 32;
	temp.getGDDThresholds()[1] = 100;
	temp.getGDDThresholds()[2] = 5;
	temp.getGDDThresholds()[3] = 10;
	EXPECT_EQ(temp.Transition(7, 35, 70), true);

}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}