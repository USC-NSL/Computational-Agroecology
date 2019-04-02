#include "terrain.h"

Terrain::Terrain() {
    for ( int i = 0; i < 10000; i++ ) {
        terrain[i].occupied = false;
    }
}

Terrain::~Terrain() {}

