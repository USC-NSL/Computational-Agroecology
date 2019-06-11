#include "plant.h"

namespace environment {

const int kInitialHealth = 10;

Plant::Plant(const std::string& type_name)
    : type_name_(type_name),
      health_(kInitialHealth),
      flowering_(false),
      accumulated_gdd_(0),
      maturity_(SEED) {}

void Plant::IncrementMaturity() {
  if (maturity_ == Plant::SEED)
    maturity_ = Plant::SEEDLING;
  else if (maturity_ == Plant::SEEDLING)
    maturity_ = Plant::JUVENILE;
  else if (maturity_ == Plant::JUVENILE)
    maturity_ = Plant::MATURE;
  else if (maturity_ == Plant::MATURE)
    maturity_ = Plant::OLD;
}

}  // namespace environment