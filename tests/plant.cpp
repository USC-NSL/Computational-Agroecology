// Copyright 2019
#include <gtest/gtest.h>
#include "../common/plant_type.cc"
#include "../common/plant_index.h"
#include "../common/corn.h"
#include "../common/squash.h"
#include "../common/bean.h"


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

TEST(Produce,  One_Produce)
{
    Corn *corn = new Corn();

    corn->addProduce(true, false, 0.8);
    // checking sizes of vectors when one added 
    EXPECT_EQ(1, corn->getProduceOnPlant().size());
    EXPECT_EQ(0, corn->getHarvestedProduce().size());
    delete corn;
}

TEST(Produce, Produce_Information)
{
    Corn *corn = new Corn();

    corn->addProduce(true, false, 0.8);
    // double checking contents of fruit object
    EXPECT_EQ(true, corn->getProduceOnPlant()[0]->ripened);
    EXPECT_EQ(false, corn->getProduceOnPlant()[0]->rotten);
    EXPECT_EQ(0.8, corn->getProduceOnPlant()[0]->weight);
}
TEST(Produce, Adding_Two_Produce)
{
    Corn *corn = new Corn();

    corn->addProduce(true, false, 0.8);
    // tree now has two produce on it
    corn->addProduce(true, true, 1.9);
    EXPECT_EQ(2, corn->getProduceOnPlant().size());
    EXPECT_EQ(0, corn->getHarvestedProduce().size());

    corn->harvestProduce();
    EXPECT_EQ(0, corn->getProduceOnPlant().size());
    EXPECT_EQ(2, corn->getHarvestedProduce().size());

}
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
