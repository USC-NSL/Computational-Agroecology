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
    Corn corn;
    AddCrop addCrop = AddCrop(corn, 0, 0);
}
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
