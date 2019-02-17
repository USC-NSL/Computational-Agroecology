// Copyright 2019
#include "plantsSelector.h"

// Get qualified plants that's qualified according to the temperature and weather
std::vector<std::string> plantsSelector::getQualifiedPlants()
{
    std::vector<std::string> plantsString;

    for(int i = 0; i < plantTypesVector.size(); i++) {
        PlantType plant = plantTypesVector[i];

        if (plant.max_absolute_temperature() >= weather_.max_temperature_of_year() and
        plant.min_absolute_temperature() <= weather_.min_temperature_of_year() and
        plant.max_absolute_rainfall() >= weather_.max_rainfall_of_year() and
        plant.min_absolute_rainfall() <= weather_.min_rainfall_of_year())
        {
            plantsString.push_back(plant.name());
        }
    }

    return  plantsString;
}

// Get optimal plants according max and min temperature and rainfall
std::vector<std::string> plantsSelector::getOptimalPlants()
{
    std::vector<std::string> plantsString;

    for(int i = 0; i < plantTypesVector.size(); i++) {
        PlantType plant = plantTypesVector[i];
        if (
            plant.max_optimal_temperature() >= weather_.max_temperature_of_year() and
            plant.min_optimal_temperature() <= weather_.min_temperature_of_year() and
            plant.max_optimal_rainfall() >= weather_.max_rainfall_of_year() and
            plant.min_optimal_rainfall() <= weather_.min_rainfall_of_year())
        {
            plantsString.push_back(plant.name());
        }
    }

    return  plantsString;
}
