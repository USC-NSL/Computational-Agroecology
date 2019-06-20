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

void Plant::IncrementProduceMaturity(Produce* temp) {
  if (temp->maturity_ == Produce::FLOWER)
    temp->maturity_ = Produce::YOUNG;
  else if (temp->maturity_ == Produce::YOUNG)
    temp->maturity_ = Produce::RIPE;
  else if (temp->maturity_ == Produce::RIPE)
    temp->maturity_ = Produce::OLD;
}

// harvest only the ready produce from the plant
void Plant::harvestReadyProduce() {
  std::set<Produce*>::iterator it = produce_.begin();
  // go through each element in set and see maturity for harvesting purposes
  for(it; it != produce_.end(); it++) {
    // if ripe, remove from plant and add to harvested set
    if( (*it)->maturity_ == Produce::RIPE) {
      produceHarvested.insert(*it);
      produce_.erase(*it);
    }
    // if old, just remove from plant
    else if((*it)->maturity_ == Produce::OLD) {
      produce_.erase(*it);
    }
  }
}

}  // namespace environment