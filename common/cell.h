//
//  cell.hpp
//  
//
//  Created by Julia Cordero on 2/3/19.
//

#ifndef cell_hpp
#define cell_hpp

#include <stdio.h>
#include "plant.h"
#include "soil.h"

struct Cell {
    Plant* plant;
    bool occupied;
    Soil* soil;
};

#endif /* cell_hpp */
