#include "plant.h"

namespace environment {

int Plant::Harvest() {
  int ret = produce_;
  produce_ = 0;
  return ret;
}

}  // namespace environment