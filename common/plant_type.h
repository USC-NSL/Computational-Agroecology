// Copyright 2019
#ifndef AGROECOLOGY_COMMON_PLANT_TYPE_H_
#define AGROECOLOGY_COMMON_PLANT_TYPE_H_

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "plant_index.h"
#include "produce.h"

// Represents a single type of plant.
class PlantType {
 public:
  // TODO: Fix this class's format.
  PlantType(){
      name_ = "";
  }

  PlantType(std::string name, int maxOT, int minOT, int maxAT, int minAT)
      : name_(std::move(name)),
        max_optimal_temperature_(maxOT),
        min_optimal_temperature_(minOT),
        max_absolute_temperature_(maxAT),
        min_absolute_temperature_(minAT) {}

  PlantType(std::string name, int maxOT, int minOT, int maxAT, int minAT, char display_symbol)
          : name_(std::move(name)),
            max_optimal_temperature_(maxOT),
            min_optimal_temperature_(minOT),
            max_absolute_temperature_(maxAT),
            min_absolute_temperature_(minAT),
            display_symbol_(display_symbol) {
      plant_index_ = new PlantIndex();

      : name_(std::move(name)), max_optimal_temperature_(maxOT),
        min_optimal_temperature_(minOT), max_absolute_temperature_(maxAT),
        min_absolute_temperature_(minAT) { produceWeight = 0.0;}

  PlantType(std::string name, int maxOT, int minOT, int maxAT, int minAT,
            char display_symbol)
      : name_(std::move(name)), max_optimal_temperature_(maxOT),
        min_optimal_temperature_(minOT), max_absolute_temperature_(maxAT),
        min_absolute_temperature_(minAT), display_symbol_(display_symbol) {
    plant_index_ = new PlantIndex();
    produceWeight = 0.0;
  }

  PlantType(std::string plantName, int maxOT, int minOT, int maxAT, int minAT,
            int maxAR, int minAR, int maxOR, int minOR, int* threshold,
            bool cultivar = false, int base = 0)
      : name_(plantName),
        max_optimal_temperature_(maxOT),
        min_optimal_temperature_(minOT),
        max_absolute_temperature_(maxAT),
        min_absolute_temperature_(minAT),
        max_absolute_annual_rainfall_(maxAR),
        min_absolute_annual_rainfall_(minAR),
        max_optimal_annual_rainfall_(maxOR),
        min_optimal_annual_rainfall_(minOR),
        cultivar_(cultivar),
        gdd_thresholds_(threshold),
        // std::array<int,5> gdd_thresholds_ = Thresholds,
        // //gdd_thresholds_(Thresholds),
        base_temp_(base) { produceWeight = 0.0;}
  // Returns true upon success.

  std::string name() { return name_; }
  int* gdd_thresholds() { return gdd_thresholds_; }
  int base_temp() { return base_temp_; }
  int max_optimal_temperature() { return max_optimal_temperature_; }
  int min_optimal_temperature() { return min_optimal_temperature_; }
  int max_absolute_temperature() { return max_absolute_temperature_; }
  int min_absolute_temperature() { return min_absolute_temperature_; }
  int max_absolute_annual_rainfall() { return max_absolute_annual_rainfall_; }
  int min_absolute_annual_rainfall() { return min_absolute_annual_rainfall_; }
  int max_optimal_annual_rainfall() { return max_optimal_annual_rainfall_; }
  int min_optimal_annual_rainfall() { return min_optimal_annual_rainfall_; }
  bool cultivar() { return cultivar_; }
  char display_symbol() { return display_symbol_; }
  PlantIndex *plant_index() { return plant_index_; }


  std::vector<Produce*> getProduceOnPlant() { return produceOnPlant; };
  std::vector<Produce*> getHarvestedProduce() { return harvestedProduce; };

  void addProduce(bool ripeP, bool poisonedP, double weightP, double yieldFactor);
  void harvestProduce();
  void increasePWeight(double weights) { this->produceWeight += weights; };
  // given an index in vector, increase its weight
  void updateProduceWeight(int index, double increase);
  // update number of produce to new status from the vector on tree
  void updateRipeStatus(bool status, int numberChanged);
  void updateRottenStatus(bool status, int numberChanged);
  double getProduceWeight() { return this->produceWeight; };

protected:
  // the name of the single species or single cultivar
  std::string name_;

  int* gdd_thresholds_;

  PlantIndex *plant_index_; // describe the index for individual plant to track their status

  int base_temp_;  // base temperature for Growing Degree Day calculations

  int max_optimal_temperature_;  // max optimal temperature requirement for
                                 // plant type

  int min_optimal_temperature_;  // min optimal temperature requirement for
                                 // plant type

  int max_absolute_temperature_;  // max absolute temperature requirement for
                                  // plant type

  int min_absolute_temperature_;  // min absolute temperature requirement for
                                  // plant type

  int max_absolute_annual_rainfall_;  // max absolute rainfall requirement for
                                      // plant type

  int min_absolute_annual_rainfall_;  // min absolute rainfall requirement for
                                      // plant type

  int max_optimal_annual_rainfall_;  // max optimal rainfall requirement for
                                     // plant type

  int min_optimal_annual_rainfall_;  // max optimal rainfall requirement for
                                     // plant type

  char display_symbol_; //symbol for displaying


  bool cultivar_;  //  specify whether this is a single species or single
                   //  cultivar.

  // JUST ADDED ON 5/16/19 TO ACCOUNT FOR FRUITS HARVESTED AND GROWN ON AN INDIVIDUAL TREE

  std::vector<Produce*>produceOnPlant; // produce on tree

  std::vector<Produce*>harvestedProduce; // produce harvested from tree

  double produceWeight;
};


#endif // AGROECOLOGY_COMMON_PLANT_TYPE_H_
