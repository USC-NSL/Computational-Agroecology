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
// harvest only the ready produce from the plant
void Plant::harvestReadyProduce()
{
  for(unsigned int i = 0; i < this->produce_.size(); i++)
  {
    // if old, dump 
    if(this->produce_[i]->getMaturity() == Produce::OLD)
    {
      this->produce_.erase(this->produce_.begin()+i);
      delete this->produce_[i];
    } // if ripe, harvest
    else if(this->produce_[i]->getMaturity() == Produce::RIPE)
    {
      this->produceWeightProduced += this->produce_[i]->getWeight(); // add weight to total harvested
      this->produceHarvested.push_back(produce_[i]); // add to harvested vector
      this->produce_.erase(this->produce_.begin()+i); // remove index from vector
    }
  }
}

}  // namespace environment