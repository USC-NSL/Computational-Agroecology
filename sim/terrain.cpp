#include "terrain.h"

Terrain::Terrain(unsigned long size) : terrain_(size) {
    for ( int i = 0; i < size; i++ ) {
        terrain_[i].resize(size);
        for ( int j = 0; j < size; j++ ) {
            terrain_[i][j].occupied_ = false;
        }
    }
}

Terrain::~Terrain() {}
