// Copyright 2019

#include <gtest/gtest.h>
#include <iostream>
#include "../common/config.h"
#include "../common/location.h"
#include "../common/action_adapter.h"


/*
 * This test suite is for testing the Config object
*/

// Tests the plant vector in the Config object
TEST(ConfigTest, ConfigPlants)
{

    Location loc = Location(100, 200);

    std::vector<PlantType> v;

    PlantType p1 = PlantType("plant 1", 0, 1, 2, 3);
    v.push_back(p1);
    PlantType p2 = PlantType("plant 2", 0, 1, 2, 3);
    v.push_back(p2);
    PlantType p3 = PlantType("plant 3", 0, 1, 2, 3);
    v.push_back(p3);
    PlantType p4 = PlantType("plant 4", 0, 1, 2, 3);
    v.push_back(p4);
    PlantType p5 = PlantType("plant 5", 0, 1, 2, 3);
    v.push_back(p5);

    Config config = Config(loc, v);

    EXPECT_EQ(config.plants().size(), 5);

    EXPECT_EQ("plant 1", config.plants()[0].name());
    EXPECT_EQ("plant 2", config.plants()[1].name());
    EXPECT_EQ("plant 3", config.plants()[2].name());
    EXPECT_EQ("plant 4", config.plants()[3].name());
    EXPECT_EQ("plant 5", config.plants()[4].name());



}

//Tests the Location in the Config object
TEST(ConfigTest, ConfigLocation)
{

    Location loc = Location(100, 200);
    std::vector<PlantType> v;
    Config config = Config(loc, v);

    EXPECT_EQ(100, config.location().latitude());
    EXPECT_EQ(200, config.location().longitude());

}

//Test the weather vector in the config
TEST(ConfigTest, ConfigYearlyWeather)
{
Location loc = Location(100, 200);
std::vector<PlantType> v;
Config config = Config(loc, v);

EXPECT_EQ(config.yearly_weather().size(), 365);

}

//Test add action function in the config
TEST(ConfigTest, AddAction)
{
Location loc = Location(100, 200);
std::vector<PlantType> v;
Config config = Config(loc, v);
Corn corn;
AddCrop addCrop1 = AddCrop(corn, 0, 0);

config.add_daily_action(addCrop1);
EXPECT_EQ(config.daily_actions().size(), 1);

}

//Test the terrain in the config
TEST(ConfigTest, ConfigTerrain)
{
Location loc = Location(100, 200);
std::vector<PlantType> v;
Config config = Config(loc, v);

EXPECT_EQ(config.terrain().width(), 100);
EXPECT_EQ(config.terrain().width(), 100);

}

//Test the perform actions part in the config
TEST(ConfigTest, PerformActions)
{
Location loc = Location(100, 200);
std::vector<PlantType> v;
Config config = Config(loc, v);
Corn corn;
AddCrop addCrop1 = AddCrop(corn, 0, 0);
config.add_daily_action(addCrop1);
EXPECT_EQ(1, config.perform_daily_actions());

}

//Test the action is actually configured
TEST(ConfigTest, PerformAction)
{
Location loc = Location(100, 200);
std::vector<PlantType> v;
Config config = Config(loc, v);
Corn corn;
AddCrop addCrop1 = AddCrop(corn, 0, 0);
config.add_daily_action(addCrop1);
EXPECT_EQ(false, config.terrain().tiles()[0][0].occupied);
EXPECT_EQ(1, config.perform_daily_actions());
EXPECT_EQ(true, config.terrain().tiles()[0][0].occupied);


}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
