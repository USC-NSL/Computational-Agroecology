//
// Created by xiao on 1/31/19.
//

#ifndef COMPUTATIONAL_AGROECOLOGY_PLANTTYPE_H
#define COMPUTATIONAL_AGROECOLOGY_PLANTTYPE_H

#include <string>

class plantType {
public:
    std::string name;
    int maxOptimalTemperature;
    int minOptimalTemperature;
    int maxAbsoluteTemperature;
    int minAbsoluteTemperature;
    int maxAbsoluteRainfall;
    int minAbsoluteRainfall;
    int maxOptimalRainfall;
    int minOptimalRainfall;

    plantType(std::string plantName, int MaxOT, int MinOT, int MaxAT, int MinAT) :
    name(plantName), maxOptimalTemperature(MaxOT), minOptimalTemperature(MinOT), maxAbsoluteTemperature(MaxAT),
    minAbsoluteTemperature(MinAT){ }
};


#endif //COMPUTATIONAL_AGROECOLOGY_PLANTTYPE_H
