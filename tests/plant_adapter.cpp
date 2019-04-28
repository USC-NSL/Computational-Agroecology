// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../common/action_adapter.h"

/*
 * This test suites is for testing all the tests related to data
 * handling in ../data.
*/

// Test the output of the Data handler that transfer csv file to 2-d array
TEST(PlantAdapter, ActionType)
{
    Corn *corn;
    AddCrop addCrop = AddCrop(corn, 0, 0);
}

//Test the perform action function without config
TEST(PlantAdapter, ActionPerform) {
    Corn *corn = new Corn();
    AddCrop addCrop = AddCrop(corn, 0, 0);
    Terrain *terrain = new Terrain(100, 100);
    std::vector<PlantType> plants;
    EXPECT_EQ((*terrain).tiles_[0][0].occupied, false);
    addCrop.perform_action(terrain, plants);
    EXPECT_EQ((*terrain).tiles_[0][0].occupied, true);
    EXPECT_EQ((*terrain).tiles_[0][0].plant, corn);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
