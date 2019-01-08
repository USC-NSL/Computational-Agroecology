#include <iostream>
using namespace std;

class Plant {

    int id, curState, yield, gdd, liveState, deadState;
    bool living, flowering;
    int l_states[][]; // see if there is better data structure\
    int d_states[][];

    public:
        Plant(int id);
        void transition(int env[]);

    };


Plant::Plant(int id)
{
    this.id = id;


    // Populate states data structure based on database

    living = true;
    flowering = false;
    dead = false;
}

Plant::void transition(int env[])
{
    // if env fulfills needs then increment liveState
    // if liveState reaches threshold then advance curState
    // if env does not fulfill needs of current state increment deadState
    // kill plant if deadState exceeds threshold
    if(living == 1){
        
    }
}







