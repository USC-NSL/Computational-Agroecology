#include "plant.h"

// TODO: Old code for CheckNeeds(), revive as needed and remove comment.
bool Plant::CheckNeeds(int rainfall, int minTemp, int maxTemp){
     if(rainfall >= type_.min_absolute_annual_rainfall() && rainfall <= type_.max_absolute_annual_rainfall() &&
        minTemp >= type_.min_absolute_temperature() && 
        maxTemp <= type_.max_absolute_temperature()){
         return true;
     }
     else{
         return false;
     }
}

int Plant::CalcGDD(int minTemp, int maxTemp){
     return ((minTemp + maxTemp)/2 + - type_.base_temp());
}

void Plant::Stage(int[] thresholds){
     int stage = 0;
     for(int i = 0; i < 5; ++i){
          if(accumulated_gdd_ >= thresholds[i]{
               stage = i;
          }
     }
     maturity_ = stages_[i];
}

// TODO: Incorporate this old Transition code into the below Transition function and then remove the commented code.
//    // if env fulfills needs then increment liveState
//    // if liveState reaches threshold then advance curState
//    // if env does not fulfill needs of current state increment deadState
//    // kill plant if dieState exceeds threshold
void Plant::Transition(int rainfall, int minTemp, int maxTemp) {
  // TODO: Add checks here (e.g., call CheckNeeds()).
    if(health_ == 0){
         return;
    }
    if (CheckNeeds(rainfall, minTemp, maxTemp)) {
        accumulated_gdd_ += CalcGDD(minTemp, maxTemp);
        Stage(type.gdd_thresholds());
        if(health_ < 10){
             health_++;
        }
    }
    else {
        accumulated_gdd_ += CalcGDD(minTemp, maxTemp);
        health_--;
    }
}
