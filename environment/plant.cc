#include "plant.h"

namespace environment {

int Plant::Harvest() {
  int ret = produce_;
  produce_ = 0;
  return ret;
}

bool Plant::operator==(const Plant& rhs){
  for(int i=0;i<kDims;i++){
    if(this->position_[i] != rhs.position_[i])
    return false;
  }
  return true;
}

}  // namespace environment