#include <gtest/gtest.h>
#include <iostream>
#include "../agent/plant_classifier/plantsSelector.h"
#include "../agent/plant_classifier/plantsSelector.cpp"

/*
 * This test suites is for testing all the tests related to data
 * handling in ../data.
*/

// Tests the plantTypeVector to see the weather passed in successfully
TEST(PlantTypes, GetAllPlantTypes)
{
    Location loc = Location(100, 100);
    Weather weather = Weather(loc, 0, 0, 0, 0, 0, 0, 0);
    plantsSelector plSelector(weather);
    // Creating an object of CSVWriter
    std::vector<PlantType> plantTypesVector = plSelector.all_plants_record();

    EXPECT_NE(plantTypesVector.size(), 0);
    EXPECT_EQ("Zoysia matrella", plantTypesVector[plantTypesVector.size() - 1].name());
    EXPECT_EQ(18, plantTypesVector[plantTypesVector.size() - 1].min_optimal_temperature());
    EXPECT_EQ(26, plantTypesVector[plantTypesVector.size() - 1].max_optimal_temperature());
    EXPECT_EQ(12, plantTypesVector[plantTypesVector.size() - 1].min_absolute_temperature());
    EXPECT_EQ(30, plantTypesVector[plantTypesVector.size() - 1].max_absolute_temperature());

}

// See the function getTypesVector is actually working
TEST(PlantTypes, WeatherPassingIn)
{
    Location loc = Location(100, 100);
    Weather weather = Weather(loc, 100, -100, 0, 0, 0, 0, 0);
    plantsSelector plSelector(weather);
    std::vector<PlantType> plantTypesVector = plSelector.all_plants_record();
    std::vector<std::string> plants = plSelector.getQualifiedPlants();
    EXPECT_NE(plantTypesVector.size(), 0);
    EXPECT_EQ(plants.size(),0);

}

// Tests getQualifiedPlants which returns all the plants that's reach the requirement
// for rainfall and temperature
TEST(PlantTypes, getQualifiedPlants)
{
    Location loc = Location(100, 100);
    Weather weather = Weather(loc, 20,15, 0, 0, 0, 1000, 800);
    plantsSelector plSelector(weather);
    std::vector<PlantType> plantTypesVector = plSelector.all_plants_record();
    std::vector<std::string> plants = plSelector.getQualifiedPlants();
    EXPECT_NE(plantTypesVector.size(), 0);
    EXPECT_NE(plants.size(),0);
}

// Tests getQualifiedPlants which returns all the plants that's reach the optimal status
// for rainfall and temperature
TEST(PlantTypes, getOptimalPlants)
{
    Location loc = Location(100, 100);
    Weather weather = Weather(loc, 20,15, 0, 0, 0, 1000, 800);
    plantsSelector plSelector(weather);
    std::vector<PlantType> plantTypesVector = plSelector.all_plants_record();
    std::vector<std::string> plants = plSelector.getOptimalPlants();
    EXPECT_NE(plantTypesVector.size(), 0);
    EXPECT_NE(plants.size(),0);
}




int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
