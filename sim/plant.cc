#include <iostream>
#include "plant.h"

using namespace std;

Plant::Plant(int id)
{
    this.id = id;


    // Populate states data structure based on database
    live_state = 0;
    dead_state = 0;
    living = true;
    flowering = false;
    dead = false;
}

//flesh this function out as necessary
//assume water and nutrient data are stored in env 1 and 2 respectively
bool Plant::CheckNeeds(int env[]){
    if(env[1] >= water_needs && env[2] >= nutrient_needs){
        return true;
    }
    else{
        return false;
    }
}
void Plant::Transition(int env[])
{
    // if env fulfills needs then increment liveState
    // if liveState reaches threshold then advance curState
    // if env does not fulfill needs of current state increment deadState
    // kill plant if dieState exceeds threshold
    if(living == 1){
        if(CheckNeeds(env[])){
            live_State++;
        }
        else{
            dead_state++;
        }
    }
}







