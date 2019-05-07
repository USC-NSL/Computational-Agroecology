// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../common/plant_index.h"
#include "../common/plant_index.cc"
#include "../common/corn.h"
#include "../common/squash.h"
#include "../common/bean.h"
#include <string>


/*
 * This test suites is for testing Plant index and plant index functionality
*/

// Test the initialization of Plant
TEST(Plant, PlantIndexInitialization)
{
PlantIndex p = PlantIndex();
EXPECT_EQ(p.health(), 10);
}

TEST(Plant, PlantIndexInPlantType)
{
Corn corn;
EXPECT_EQ(corn.name(),"corn");
EXPECT_EQ(corn.plant_index()->health(), 10);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//
// Created by xiao on 4/28/19.
//

