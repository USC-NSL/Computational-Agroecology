// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../sim/terrain.h"

// Tests the plantTypeVector to see the weather passed in successfully
TEST(Terrain, TerrainInitializer)
{
  Terrain newTerrain(100, 100);
  EXPECT_EQ(newTerrain.length(), 100);
  EXPECT_EQ(newTerrain.width(), 100);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}