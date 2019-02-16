// Copyright 2019
#include "plantsSelector.h"

// Get qualified plants that's qualified according to the temperature and weather
std::vector<std::string> plantsSelector::getQualifiedPlants()
{
    std::vector<std::string> plantsString;

    for(int i = 0; i < plantTypesVector.size(); i++) {
        plantType plant = plantTypesVector[i];

        if (plant.maxAbsoluteTemperature >= weather_.MaxTemperatureOfYear_ and
        plant.minAbsoluteTemperature <= weather_.MinTemperatureOfYear_ and
        plant.maxAbsoluteRainfall >= weather_.MaxRainfall_ and
        plant.minAbsoluteRainfall <= weather_.MinRainfall_)
        {
            plantsString.push_back(plant.name);
        }
    }

    return  plantsString;
}

// Get optimal plants according max and min temperature and rainfall
std::vector<std::string> plantsSelector::getOptimalPlants()
{
    std::vector<std::string> plantsString;

    for(int i = 0; i < plantTypesVector.size(); i++) {
        plantType plant = plantTypesVector[i];
        if (
            plant.maxOptimalTemperature >= weather_.MaxTemperatureOfYear_ and
            plant.minOptimalTemperature <= weather_.MinTemperatureOfYear_ and
            plant.maxOptimalRainfall >= weather_.MaxRainfall_ and
            plant.minOptimalRainfall <= weather_.MinRainfall_)
        {
            plantsString.push_back(plant.name);
        }
    }

    return  plantsString;
}