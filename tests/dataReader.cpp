// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../data/CSVReader.h"
#include "../data/CSVReader.cpp"
#include "../common/climate.h"
#include "../common/climate.cpp"

/*
 * This test suites is for testing all the tests related to data
 * handling in ../data.
*/

// Test the output of the Data handler that transfer csv file to 2-d array
TEST(DataTest, DataHandlerOutput)
{
    // Creating an object of CSVWriter
    CSVReader reader("../data/PlantEnvironment.csv");

    // Get the data from CSV File
    std::vector<std::vector<std::string> > dataList = reader.getData();
    EXPECT_NE(dataList.size(), 0);
    std::string lastComponentName = dataList[dataList.size() - 1][0];
    EXPECT_EQ("Zoysia matrella", lastComponentName);
    // int MaxOT, int MinOT, int MaxAT, int MinAT
}

// Test the getPlantTypes function to see if it transfer the data into plantType
TEST(DataAdapterPlant, DataAdapterPlantOutput)
{
    CSVReader reader("../data/PlantEnvironment.csv");
    // Creating an object of CSVWriter
    std::vector<PlantType> plantTypesVector = getPlantTypes();


    EXPECT_NE(plantTypesVector.size(), 0);
    //std::string lastComponentName = dataList[dataList.size() - 1][0];
    EXPECT_EQ("Zoysia matrella", plantTypesVector[plantTypesVector.size() - 1].name());
    EXPECT_EQ(18, plantTypesVector[plantTypesVector.size() - 1].min_optimal_temperature());
    EXPECT_EQ(26, plantTypesVector[plantTypesVector.size() - 1].max_optimal_temperature());
    EXPECT_EQ(12, plantTypesVector[plantTypesVector.size() - 1].min_absolute_temperature());
    EXPECT_EQ(30, plantTypesVector[plantTypesVector.size() - 1].max_absolute_temperature());

}
// Test the data adapter to see if the rainfall passing in successfully
TEST(DataAdapterPlant, DataAdapterPlantOutputWithRainfall)
{
    CSVReader reader("../data/PlantEnvironment.csv");
    // Creating an object of CSVWriter
    std::vector<PlantType> plantTypesVector = getPlantTypes();

    EXPECT_EQ("Zoysia matrella", plantTypesVector[plantTypesVector.size() - 1].name());
    EXPECT_EQ(1000, plantTypesVector[plantTypesVector.size() - 1].min_optimal_annual_rainfall());
    EXPECT_EQ(1700, plantTypesVector[plantTypesVector.size() - 1].max_optimal_annual_rainfall());
    EXPECT_EQ(700, plantTypesVector[plantTypesVector.size() - 1].min_absolute_annual_rainfall());
    EXPECT_EQ(2600, plantTypesVector[plantTypesVector.size() - 1].max_absolute_annual_rainfall());

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
