// Copyright 2019
#include <gtest/gtest.h>
#include <iostream>
#include "../common/bean.h"
#include "../sim/terrain.h"
#include <stdio.h>
#include "../common/config.h"

// Tests the plantTypeVector to see the weather passed in successfully
TEST(Terrain, TerrainInitializer)
{
  Terrain newTerrain(100, 100);
  EXPECT_EQ(newTerrain.length(), 100);
  EXPECT_EQ(newTerrain.width(), 100);
}

TEST(Terrain, TerrainCellInitializer)
{
    Terrain newTerrain(100, 100);
    EXPECT_EQ(newTerrain.tiles()[0][0].occupied, false);
    EXPECT_EQ(newTerrain.tiles()[0][0].plant, nullptr);
    EXPECT_NE(newTerrain.tiles()[0][0].soil, nullptr);
    EXPECT_EQ(newTerrain.tiles()[0][0].soil->pH(), 7);
}


TEST(Terrain, Cell)
{
    cellValue newCell({false, 1, NULL, NULL});
    newCell.occupied = true;
    EXPECT_EQ(newCell.occupied, true);
}

TEST(Terrain, CellTiles)
{
    std::vector<std::vector<cellValue>> tiles;
    tiles.resize(100, std::vector<cellValue>(100, {false, 1, NULL, NULL}));
    tiles[0][0].occupied = true;
    EXPECT_EQ(tiles[0][0].occupied, true);
}

TEST(Terrain, TerrainTiles)
{
    Terrain newTerrain(100, 100);
    EXPECT_EQ(newTerrain.tiles().size(), 100);
}

TEST(Terrain, TerrainTilesWidth)
{
    Terrain newTerrain(100, 100);
    EXPECT_EQ(newTerrain.tiles()[0].size(), 100);
}

TEST(Terrain, TerrainChangePtr)
{
    Terrain newTerrain(100, 100);
    EXPECT_EQ(newTerrain.tiles_[0][0].plant, nullptr);
    Bean bean;
    newTerrain.tiles_[0][0].plant = &bean;
    EXPECT_EQ(newTerrain.tiles_[0][0].plant, &bean);
}

TEST(Terrain, TerrainChange)
{
    Terrain newTerrain(100, 100);
    EXPECT_EQ(newTerrain.tiles_[0][0].occupied, false);
    newTerrain.tiles_[0][0].occupied = true;
    EXPECT_EQ(newTerrain.tiles_[0][0].occupied, true);
}

TEST(Terrain, TerrainPointerWork)
{
    Terrain newTerrain(100, 100);
    Terrain *terrainPtr = &newTerrain;
    Terrain terrain = *terrainPtr;
    terrain.tiles_[0][0].occupied = true;
    EXPECT_EQ(newTerrain.tiles_[0][0].occupied, false);
}

TEST(Terrain, TerrainPointerWorkCorrect)
{
    Terrain newTerrain(100, 100);
    Terrain *terrainPtr = &newTerrain;
    terrainPtr->tiles_[0][0].occupied = true;
    EXPECT_EQ(newTerrain.tiles_[0][0].occupied, true);
}

TEST(Terrain, TerrainPointerWorkPointerCorrect)
{
    Terrain newTerrain(100, 100);
    Terrain *terrainPtr = &newTerrain;
    Terrain *anotherPtr;
    anotherPtr = terrainPtr;
    anotherPtr->tiles_[0][0].occupied = true;
    EXPECT_EQ(newTerrain.tiles_[0][0].occupied, true);
}

TEST(Terrain, TerrainNew)
{
    Terrain *terrain;
    terrain = new Terrain(100, 100);
    EXPECT_EQ((*terrain).tiles_[0][0].occupied, false);
    EXPECT_EQ((*terrain).tiles()[0].size(), 100);
}

TEST(Terrain, TerrainNewPtr)
{
    Terrain *terrain;
    terrain = new Terrain(100, 100);
    Terrain *newTerrainPtr = terrain;
    EXPECT_EQ((*newTerrainPtr).tiles_[0][0].occupied, false);
    (*newTerrainPtr).tiles_[0][0].occupied = true;
    EXPECT_EQ((*newTerrainPtr).tiles()[0].size(), 100);
    EXPECT_EQ((*terrain).tiles_[0][0].occupied, true);
}

TEST(Terrain, TerrainPrintSimpleTests)
{
    Terrain *terrain;
    terrain = new Terrain(10, 10);
    for(int i = 0; i < terrain->tiles().size(); i++)
    {
        for (int j = 0; j < terrain->tiles()[0].size(); j++)
        {
            if(terrain->tiles()[i][j].occupied) {
                printf("*");
            }else {
                printf("-");
            }
        }
        printf("\n");
    }
}

TEST(Terrain, TerrainPrintSimpleComplex)
{
    Terrain *terrain;
    terrain = new Terrain(10, 10);
    Corn *corn = new Corn();
    AddCrop *addCrop1 = new AddCrop(corn, 0, 0);
    std::vector<PlantType> plants;
    (*addCrop1).perform_action(terrain, plants);
    terrain->terrain_display();
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}