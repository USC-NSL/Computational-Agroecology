#include <gtest/gtest.h>
#include <iostream>
#include "../data/CSVReader.h"
#include "../data/CSVReader.cpp"

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
}

TEST(DataAdapterPlant, DataAdapterPlantOutput)
{
    CSVReader reader("../data/PlantEnvironment.csv");
    // Creating an object of CSVWriter
    std::vector<plantType> plantTypesVector = getPlantTypes();


    EXPECT_NE(plantTypesVector.size(), 0);
    //std::string lastComponentName = dataList[dataList.size() - 1][0];
    EXPECT_EQ("Zoysia matrella", plantTypesVector[plantTypesVector.size() - 1].name);
    EXPECT_EQ(18, plantTypesVector[plantTypesVector.size() - 1].maxOptimalTemperature);
    EXPECT_EQ(26, plantTypesVector[plantTypesVector.size() - 1].minOptimalTemperature);
    EXPECT_EQ(18, plantTypesVector[plantTypesVector.size() - 1].maxAbsoluteTemperature);
    EXPECT_EQ(30, plantTypesVector[plantTypesVector.size() - 1].minAbsoluteTemperature);

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
