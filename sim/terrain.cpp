// Copyright 2019
#include "terrain.h"
void Terrain::terrain_display() {
    for(int i = 0; i < tiles_.size(); i++)
    {
        for (int j = 0; j < tiles_[0].size(); j++)
        {
            if(tiles_[i][j].plant) {
                PlantType *plant = tiles_[i][j].plant;
                printf("%c", (*plant).display_symbol());
            } else {
                printf("*");
            }
        }
        printf("\n");
    }
}