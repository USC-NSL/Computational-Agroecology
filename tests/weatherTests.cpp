// Copyright 2019

#include <gtest/gtest.h>
#include <iostream>
#include "vector"
#include "../common/weather.h"

/*
 * This test suites is for testing all the tests related to data
 * handling in ../data.
*/

// Tests the plantTypeVector to see the weather passed in successfully
TEST(Weather, MakeWeatherArray)
{
   Weather weather(15, 26, 1);
   std::vector<Weather> yearlyWeather;
   for(int i = 0; i < 365; i++)
    {
       yearlyWeather.push_back(weather);
    }
   EXPECT_EQ(yearlyWeather.size(), 365);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}