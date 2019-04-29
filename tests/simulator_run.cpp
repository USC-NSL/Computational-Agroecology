//Copyright 2019

// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../common/config.h"
#include "../common/config.cpp"
/*
 * This test suites is for testing all the tests related to allow simulator to run
*/

// Test the output of state for each day in a week. a new corn will be planted each day in (i ,i)
TEST(Simulator, SimulatorMonitorForWeek)
{
    Location loc = Location(100, 200);
    std::vector<PlantType> v;
    Config config = Config(loc, v);
    for(int i = 0; i <= 7; i++) {
        Corn *corn = new Corn();
        AddCrop *addCrop1 = new AddCrop(corn, i, i);
        config.add_daily_action(addCrop1);
        config.perform_daily_actions();
        config.state_display(i);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


