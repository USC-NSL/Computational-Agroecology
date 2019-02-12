// Copyright 2019
#ifndef AGENT_PLANTSCLASSIFIER_PLANTSSELECTOR_H_
#define AGENT_PLANTSCLASSIFIER_PLANTSSELECTOR_H_

#include "../../weather.h"
#include <vector>
#include <iterator>
#include "../../data/CSVReader.h"
#include "../../data/CSVReader.cpp"
#include "plantType.h"
#include <string>

class plantsSelector {
 public:
    std::vector<plantType> plantTypesVector;
    plantsSelector(Weather weather,
            std::vector<plantType> plantTV = getPlantTypes()) :
    weather_(weather), plantTypesVector(plantTV) {}
    std::vector<std::string> getQualifiedPlants();
    std::vector<std::string> getOptimalPlants();
    Weather weather_;
};


#endif  // AGENT_PLANTSCLASSIFIER_PLANTSSELECTOR_H_
