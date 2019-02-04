#include "plantsSelector.h"

// Get qualified plants that won't survive during the year
std::vector<std::string> plantsSelector::getQualifiedPlants()
{
    std::vector<std::string> plantsString;
// plant.maxAbsoluteRainfall >= weather_.MaxRainfall_ and
//        plant.minAbsoluteRainfall <= weather_.MinRainfall_ and
    for(int i = 0; i < plantTypesVector.size(); i++) {
        plantType plant = plantTypesVector[i];
        // std::cout<< plant.name;
        if (plant.maxAbsoluteTemperature >= weather_.MaxTemperatureOfYear_ and
        plant.minAbsoluteTemperature <= weather_.MinTemperatureOfYear_)
        {
            plantsString.push_back(plant.name);
        }
    }
    // std::cout << "size of the qualified plants:" << plantsString.size();
    std::cout << plantsString.size();
    return  plantsString;
}

// Get optimal plants according average temperature
std::vector<std::string> plantsSelector::getOptimalPlants()
{
    std::vector<std::string> plantsString;

    for(int i = 0; i < plantTypesVector.size(); i++) {
        plantType plant = plantTypesVector[i];
        if (
            plant.maxOptimalTemperature >= weather_.MaxTemperatureOfYear_ and
            plant.minOptimalTemperature <= weather_.MinTemperatureOfYear_)
        {
            plantsString.push_back(plant.name);
        }
    }

    std::cout << plantsString.size();
    return  plantsString;
}
