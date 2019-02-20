#include "plant.h"

// TODO: Old code for CheckNeeds(), revive as needed and remove comment.
bool Plant::CheckNeeds(int rainfall, int temperature){
     if(rainfall >= type_.min_absolute_rainfall() && rainfall <= type_.max_absolute_rainfall() && 
        temperature >= type_.min_absolute_temperature() && 
        temperature <= type_.max_absolute_temperature()){
         return true;
     }
     else{
         return false;
     }
}

// TODO: Incorporate this old Transition code into the below Transition function and then remove the commented code.
//    // if env fulfills needs then increment liveState
//    // if liveState reaches threshold then advance curState
//    // if env does not fulfill needs of current state increment deadState
//    // kill plant if dieState exceeds threshold
void Plant::Transition(Maturity new_maturity) {
  // TODO: Add checks here (e.g., call CheckNeeds()).
    if(health_ == 0){
         return;
    }
    if (CheckNeeds(rainfall, temperature)) {
        maturity_ = new_maturity;
    }
    else {
        health_ --;
    }
}
