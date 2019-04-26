// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../common/plant.h"
#include "../common/plant.cc"
#include "../data/CSVReader.h"
#include "../data/CSVReader.cpp"
#include "../common/corn.h"
#include "../common/squash.h"
#include "../common/bean.h"
#include <string>


/*
 * This test suites is for testing Plant and plant functionality
*/

// Test the initialization of Plant
TEST(Plant, PlantInitialization)
{

  CSVReader reader("../data/PlantEnvironment.csv");
  // Creating an object of CSVWriter
  std::vector<PlantType> plantTypesVector = getPlantTypes();
  Plant p(plantTypesVector[0]);
  EXPECT_EQ(p.health(), 10);
}

TEST(Plant, Corn)
{
    Corn corn;
    EXPECT_EQ(corn.name(),"corn");
}

TEST(Plant, Squash)
{
Squash squash;
  EXPECT_EQ(squash.name(),"squash");
}

TEST(Plant, Bean)
{
  Bean bean;
  EXPECT_EQ(bean.name(),"bean");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
