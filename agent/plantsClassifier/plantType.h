// Copyright 2019
#ifndef AGENT_PLANTSCLASSIFIER_PLANTTYPE_H_
#define AGENT_PLANTSCLASSIFIER_PLANTTYPE_H_

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

    plantType(std::string plantName, int MaxOT, int MinOT,
            int MaxAT, int MinAT) :
    name(plantName), maxOptimalTemperature(MaxOT),
    minOptimalTemperature(MinOT), maxAbsoluteTemperature(MaxAT),
    minAbsoluteTemperature(MinAT) { }

    plantType(std::string plantName, int MaxOT, int MinOT, int MaxAT,
            int MinAT, int MaxAR, int MinAR, int MaxOR, int MinOR) :
            name(plantName), maxOptimalTemperature(MaxOT),
            minOptimalTemperature(MinOT), maxAbsoluteTemperature(MaxAT),
            minAbsoluteTemperature(MinAT), maxAbsoluteRainfall(MaxAR),
            minAbsoluteRainfall(MinAR), maxOptimalRainfall(MaxOR),
            minOptimalRainfall(MinOR) { }
};

#endif  // AGENT_PLANTSCLASSIFIER_PLANTTYPE_H_
