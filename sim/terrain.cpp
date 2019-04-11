#include "terrain.h"

Terrain::Terrain() {
    for ( int i = 0; i < 10000; i++ ) {
        for ( int j = 0; j < 10000; j++ ) {
            terrain[i][j].occupied_ = false;
        }
    }
}

Terrain::~Terrain() {}

