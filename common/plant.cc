#include "plant.h"

// TODO: Old code for CheckNeeds(), revive as needed and remove comment.
//       flesh this function out as necessary
//       assume water and nutrient data are stored in env 1 and 2
//       respectively
bool Plant::CheckNeeds(int rainfall, int temperature){
     if(rainfall >=  type_.minAbsoluteRainfall && temperature >= type_.minAbsoluteTemperature){
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
bool Plant::Transition(Maturity new_maturity) {
  // TODO: Add checks here (e.g., call CheckNeeds()).
    if (CheckNeeds(rainfall, temperature)) {
        maturity_ = new_maturity;
    }
    else {
        health_ --;
    }
  

  return true;
}
