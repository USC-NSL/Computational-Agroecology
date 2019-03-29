// Copyright 2019
#include "plantsSelector.h"

// Get qualified plants that's qualified according to the temperature and weather
std::vector<std::string> PlantsSelector::getQualifiedPlants()
{
    std::vector<std::string> plantsString;

    for(int i = 0; i < all_plants_record_.size(); i++) {
        PlantType plant = all_plants_record_[i];

        if (plant.max_absolute_temperature() >= weather_.max_temp_year() and
        plant.min_absolute_temperature() <= weather_.min_temp_year() and
        plant.max_absolute_annual_rainfall() >= weather_.max_rainfall_year() and
        plant.min_absolute_annual_rainfall() <= weather_.min_rainfall_year())
        {
            plantsString.push_back(plant.name());
        }
    }

    return  plantsString;
}

// Get optimal plants according max and min temperature and rainfall
std::vector<std::string> PlantsSelector::getOptimalPlants()
{
    std::vector<std::string> plantsString;

    for(int i = 0; i < all_plants_record_.size(); i++) {
        PlantType plant = all_plants_record_[i];
        if (
            plant.max_optimal_temperature() >= weather_.max_temp_year() and
            plant.min_optimal_temperature() <= weather_.min_temp_year() and
            plant.max_optimal_annual_rainfall() >= weather_.max_rainfall_year() and
            plant.min_optimal_annual_rainfall() <= weather_.min_rainfall_year())
        {
            plantsString.push_back(plant.name());
        }
    }

    return  plantsString;
}