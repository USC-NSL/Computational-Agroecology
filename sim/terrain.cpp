// Copyright 2019
#include "terrain.h"
void Terrain::terrain_display()
{
    for (int i = 0; i < tiles_.size(); i++)
    {
        for (int j = 0; j < tiles_[0].size(); j++)
        {
            if (tiles_[i][j].plant)
            {
                PlantType *plant = tiles_[i][j].plant;
                printf("%c", (*plant).display_symbol());
            }
            else
            {
                printf("*");
            }
        }
        printf("\n");
    }
}

PlantType *Terrain::get_plant_status(int date, int x, int y)
{
    // TODO: add date info or hour info
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < length);
    return tiles_[x][y].plant;
};