// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../common/plant_index.h"
#include "../common/plant_index.cc"
#include "../common/corn.h"
#include "../common/squash.h"
#include "../common/bean.h"
#include <string>


/*
 * This test suites is for testing Plant and plant functionality
*/

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
