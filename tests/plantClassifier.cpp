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
    Weather weather = Weather(0, 0, 0, 0);
    PlantsSelector plSelector(weather);
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
    Weather weather = Weather(100, -100, 0, 0);
    PlantsSelector plSelector(weather);
    std::vector<PlantType> plantTypesVector = plSelector.all_plants_record();
    std::vector<std::string> plants = plSelector.getQualifiedPlants();
    EXPECT_NE(plantTypesVector.size(), 0);
    EXPECT_EQ(plants.size(),0);

}

// Tests getQualifiedPlants which returns all the plants that's reach the requirement
// for rainfall and temperature
TEST(PlantTypes, getQualifiedPlants)
{
    Weather weather = Weather(20,15, 0, 0);
    PlantsSelector plSelector(weather);
    std::vector<PlantType> plantTypesVector = plSelector.all_plants_record();
    std::vector<std::string> plants = plSelector.getQualifiedPlants();
    EXPECT_NE(plantTypesVector.size(), 0);
    EXPECT_NE(plants.size(),0);
}

// Tests getQualifiedPlants which returns all the plants that's reach the optimal status
// for rainfall and temperature
TEST(PlantTypes, getOptimalPlants)
{
    Weather weather = Weather(20,15, 0, 0);
    PlantsSelector plSelector(weather);
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