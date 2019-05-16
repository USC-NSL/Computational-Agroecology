// Copyright 2019
#ifndef COMPUTATIONAL_AGROECOLOGY_FRUIT_H
#define COMPUTATIONAL_AGROECOLOGY_FRUIT_H

/* Each plant type has a crop type (ex: bean, squash, etc.) for each specific plant
and each crop type publicly inherits from this class. This class has two primary 
functions, to grow a fruit on the tree, and to harvest a fruit from the tree, which 
also removes a fruit from the tree while increasing the number harvested.
*/
class Fruit {
public:
  void growFruit(int number) { this->quantityOnTree = this->quantityOnTree + number; };
  void harvestFruit() { this->quantityHarvested += this->quantityOnTree; this->quantityOnTree=0;};
private:
	int quantityOnTree;
	int quantityHarvested;
};

#endif // COMPUTATIONAL_AGROECOLOGY_BEAN_H
