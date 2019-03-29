#include <utility>

// Copyright 2019
#ifndef AGENT_PLANTSCLASSIFIER_PLANTSSELECTOR_H_
#define AGENT_PLANTSCLASSIFIER_PLANTSSELECTOR_H_

#include "../../weather.h"
#include <vector>
#include <iterator>
#include "../../data/CSVReader.h"
#include "../../data/CSVReader.cpp"
#include "../../common/plant_type.h"
#include <string>

class PlantsSelector {
 public:
  explicit PlantsSelector(Weather weather,
            std::vector<PlantType> plantTV = getPlantTypes()) :
    weather_(weather),
    all_plants_record_(std::move(plantTV)) {}
    Weather weather() { return weather_; }
    std::vector<std::string> getQualifiedPlants();
    std::vector<std::string> getOptimalPlants();
    std::vector<PlantType> all_plants_record() { return  all_plants_record_; }

 private:
    Weather weather_;  // keep the weather information of the farm that perform selection
    std::vector<PlantType> all_plants_record_;  // keep the information of all plant types

};


#endif  // AGENT_PLANTSCLASSIFIER_PLANTSSELECTOR_H_
