// Copyright 2019
#include "plant_index.h"
#include <math.h>

// TODO: Old code for CheckNeeds(), revive as needed and remove comment.
bool PlantIndex::CheckNeeds(double rainfall, double minTemp, double maxTemp) {
  return true;
}

//TODO: All of the GDD and temperature measurements should be updated to
// doubles, not just ints. This is still a W.I.P
int PlantIndex::CalcGDD(double minTemp, double maxTemp){
     return (int) round((minTemp + maxTemp)/2 + - base_temperature_);
}

void PlantIndex::Stage(int* thresholds) {
     for(int i = 0; i < 5; ++i){
          if(accumulated_gdd_ >= thresholds[i]) {
               IncrementMaturity();
          }
     }
}

// TODO: Incorporate this old Transition code into the below Transition function and then remove the commented code.
//    // if env fulfills needs then increment liveState
//    // if liveState reaches threshold then advance curState
//    // if env does not fulfill needs of current state increment deadState
//    // kill plant if dieState exceeds threshold
bool PlantIndex::Transition(double rainfall, double minTemp, double maxTemp) {
  // TODO: Add checks here (e.g., call CheckNeeds()).
    if(health_ == 0){
         return false;
    }
    if (CheckNeeds(rainfall, minTemp, maxTemp)) {
        accumulated_gdd_ += CalcGDD(minTemp, maxTemp);
        Stage(gdd_thresholds_);
        if(health_ < 10){
             health_++;
        }
    }
    else {
        accumulated_gdd_ += CalcGDD(minTemp, maxTemp);
        health_--;
    }
    return true;
}


void PlantIndex::IncrementMaturity() {
  if (maturity_ == Maturity::SEED) maturity_ = Maturity::SEEDLING;
  else if (maturity_ == Maturity::SEEDLING) maturity_ = Maturity::JUVENILE;
  else if (maturity_ == Maturity::JUVENILE) maturity_ = Maturity::MATURE;
  else if (maturity_ == Maturity::MATURE) maturity_ = Maturity::OLD;
}
