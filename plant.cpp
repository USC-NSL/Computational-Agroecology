#include <iostream>
#incldue "plant.h"

using namespace std;

Plant::Plant(int id)
{
    this.id = id;


    // Populate states data structure based on database

    living = true;
    flowering = false;
    dead = false;
}

void Plant::transition(int env[])
{
    // if env fulfills needs then increment liveState
    // if liveState reaches threshold then advance curState
    // if env does not fulfill needs of current state increment deadState
    // kill plant if deadState exceeds threshold
    if(living == 1){
        
    }
}







