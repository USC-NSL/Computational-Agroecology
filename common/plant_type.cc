// Copyright 2019
#include "plant_type.h"

// take each produce from tree and move it to another vector for harvested
void PlantType::harvestProduce()
{
	while(produceOnPlant.size()!=0)
	{
		harvestedProduce.push_back(produceOnPlant.back()); // push produce on tree to harvested vector
		produceOnPlant.pop_back(); // pop from the vector on tree
	}
	this->produceWeight+=(harvestedProduce[harvestedProduce.size()-1])->weight;
	return;
};
// set default function for produce, that sets ripe to false, poisoned to false,
void PlantType::addProduce(bool ripeP, bool poisonedP, double weightP)
{
	if(this->name_ == "corn"){
		CornProduce* temp = new CornProduce(ripeP, poisonedP, weightP);
		this->produceOnPlant.push_back(temp);
	}
	else if(this->name_ == "squash"){
		SquashProduce* temp = new SquashProduce(ripeP, poisonedP, weightP);
		this->produceOnPlant.push_back(temp);
	}
	else if(this->name_ == "beans") {
		BeanProduce* temp = new BeanProduce(ripeP, poisonedP, weightP);
		this->produceOnPlant.push_back(temp);
	}
	return;
};
void PlantType::updateProduceWeight(int index, double increase)
{
	if(index < 0 || index >= produceOnPlant.size())
	{
		std::cerr << "Invalid index to update Produce Weight" << std::endl;
		return;
	}
	produceOnPlant[index]->updateWeight(increase);
};
void PlantType::updateRipeStatus(bool status, int numberChanged)
{
	int count = numberChanged;
	for(int i = 0; i < produceOnPlant.size(); i++)
	{
		if(count == 0) return;
		if(produceOnPlant[i]->ripened != status)
		{
			produceOnPlant[i]->ripened = status;
			count--;
		}
	}
}
void PlantType::updateRottenStatus(bool status, int numberChanged)
{
	int count = numberChanged;
	for(int i = 0; i < produceOnPlant.size(); i++)
	{
		if(count == 0) return;
		if(produceOnPlant[i]->rotten != status)
		{
			produceOnPlant[i]->rotten = status;
			count--;
		}
	}
}
