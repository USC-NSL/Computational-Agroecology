// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../common/action_adapter.h"

/*
 * This test suites is for testing all the tests related to plant adapter to perform action
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

//Test the perform add crop action function validation
TEST(PlantAdapter, AddActionValidation) {
Corn *corn = new Corn();
AddCrop addCrop = AddCrop(corn, 0, 0);
Terrain *terrain = new Terrain(100, 100);
std::vector<PlantType> plants;
EXPECT_EQ(addCrop.perform_action(terrain, plants), true);
EXPECT_EQ(addCrop.perform_action(terrain, plants), false);
}

//Test the perform remove crop action function validation
TEST(PlantAdapter, RemoveActionValidation) {
Corn *corn = new Corn();
AddCrop addCrop = AddCrop(corn, 0, 0);
RemoveCrop removeCrop = RemoveCrop(0, 0);
RemoveCrop removeCrop1 = RemoveCrop(0, 1);
Terrain *terrain = new Terrain(100, 100);
std::vector<PlantType> plants;
EXPECT_EQ(addCrop.perform_action(terrain, plants), true);
EXPECT_EQ(removeCrop.perform_action(terrain, plants), true);
EXPECT_EQ(removeCrop1.perform_action(terrain, plants), false);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
