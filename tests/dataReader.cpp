#include <gtest/gtest.h>
#include <iostream>
#include "../data/CSVReader.h"
#include "../data/CSVReader.cpp"

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


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
