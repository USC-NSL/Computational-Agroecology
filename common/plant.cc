#include "plant.h"

// TODO: Old code for CheckNeeds(), revive as needed and remove comment.
bool Plant::CheckNeeds(int rainfall, int minTemp, int maxTemp) {
  return (rainfall >= type_.min_absolute_annual_rainfall()
  && rainfall <= type_.max_absolute_annual_rainfall()
  && minTemp >= type_.min_absolute_temperature()
  && maxTemp <= type_.max_absolute_temperature());
}

int Plant::CalcGDD(int minTemp, int maxTemp){
     return ((minTemp + maxTemp)/2 + - type_.base_temp());
}

void Plant::Stage(int* thresholds) {
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
bool Plant::Transition(int rainfall, int minTemp, int maxTemp) {
  // TODO: Add checks here (e.g., call CheckNeeds()).
    if(health_ == 0){
         return false;
    }
    if (CheckNeeds(rainfall, minTemp, maxTemp)) {
        accumulated_gdd_ += CalcGDD(minTemp, maxTemp);
        Stage(type_.gdd_thresholds());
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

void Plant::IncrementMaturity() {
  if (maturity_ == Maturity::SEED) maturity_ = Maturity::SEEDLING;
  else if (maturity_ == Maturity::SEEDLING) maturity_ = Maturity::JUVENILE;
  else if (maturity_ == Maturity::JUVENILE) maturity_ = Maturity::MATURE;
  else if (maturity_ == Maturity::MATURE) maturity_ = Maturity::OLD;
}
