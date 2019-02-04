#ifndef COMPUTATIONAL_AGROECOLOGY_PLANTSSELECTOR_H
#define COMPUTATIONAL_AGROECOLOGY_PLANTSSELECTOR_H

#include "../../weather.h"
#include <vector>
#include <iterator>
#include "../../data/CSVReader.h"
#include "../../data/CSVReader.cpp"
#include "plantType.h"

class plantsSelector {

 public:
    std::vector<plantType> plantTypesVector;
    plantsSelector(Weather weather, std::vector<plantType> plantTV = getPlantTypes()) :
    weather_(weather), plantTypesVector(plantTV) {}
    std::vector<std::string> getQualifiedPlants();
    std::vector<std::string> getOptimalPlants();
    Weather weather_;

};


#endif //COMPUTATIONAL_AGROECOLOGY_PLANTSSELECTOR_H
