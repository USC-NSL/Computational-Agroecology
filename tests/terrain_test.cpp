#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include "../sim/terrain.h"

using namespace std;

/*
 * This test suite is for testing the Terrain object
 */

// Tests the boolean value of each Cell in the 2D array terrain in Terrain object
// Tests constructor of Terrain object
TEST(TerrainTest, TerrainCells)
{
    Terrain t;
    
    vector< vector<Cell>> t_terrain = t.terrain();
    
    for ( int i = 0; i < 10000; i++ ) {
        for ( int j = 0; j < 10000; j++ ) {
            EXPECT_EQ(t_terrain[i][j].occupied_, false);
        }
    }
}
