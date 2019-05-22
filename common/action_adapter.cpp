// Copyright 2019

#include "action_adapter.h"

double ActionAdapter::calculateTotalYield(Terrain *terrain) {
	double returnYield=0.0;
	for(int i = 0; i < terrain->width(); i++) {
		for(int j = 0; j < terrain->length(); j++) {
			returnYield+=(*terrain).tiles_[j][i].plant->getProduceWeight();
		}
	}
	// divide by the area of the land
	returnYield /= terrain->area();
	return returnYield;
}