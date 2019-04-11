#include "gtest/gtest.h"
#include <iostream>
#include "terrain.h"

/*
 * This test suite is for testing the Terrain object
 */

// Tests the boolean value of each Cell in the 2D array terrain in Terrain object
// Tests constructor of Terrain object
TEST(TerrainTest, TerrainCells)
{
    Terrain t();
    
    for ( int i = 0; i < 10000; i++ ) {
        EXPECT_EQ(terrain[i].occupied, false);
    }

}
