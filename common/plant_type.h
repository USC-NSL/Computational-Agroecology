#ifndef AGROECOLOGY_COMMON_PLANT_TYPE_H_
#define AGROECOLOGY_COMMON_PLANT_TYPE_H_

#include <string>

// Represents a single type of plant.

class PlantType {
 public:
 // TODO: Fix this class's format.


  PlantType(std::string name, int MaxOT, int MinOT,
          int MaxAT, int MinAT) :
  name_(name),
  max_optimal_temperature_(MaxOT),
  min_optimal_temperature_(MinOT),
  max_absolute_temperature_(MaxAT),
  min_absolute_temperature_(MinAT) { }

  PlantType(std::string plantName, int MaxOT, int MinOT, int MaxAT,
          int MinAT, int MaxAR, int MinAR,
          int MaxOR, int MinOR, int* Threshold, bool cultivar = false,
          int Base = 0) :
          name_(plantName),
          max_optimal_temperature_(MaxOT),
          min_optimal_temperature_(MinOT),
          max_absolute_temperature_(MaxAT),
          min_absolute_temperature_(MinAT),
          max_absolute_annual_rainfall_(MaxAR),
          min_absolute_annual_rainfall_(MinAR),
          max_optimal_annual_rainfall_(MaxOR),
          min_optimal_annual_rainfall_(MinOR),
          cultivar_(cultivar),
          gdd_thresholds_(Threshold),
          //std::array<int,5> gdd_thresholds_ = Thresholds, //gdd_thresholds_(Thresholds),
          base_temp_(Base){ }


  std::string name() { return  name_; }
  int* gdd_thresholds() { return gdd_thresholds_; }
  int base_temp() { return base_temp_; }
  int max_optimal_temperature() { return max_optimal_temperature_; }
  int min_optimal_temperature() { return min_optimal_temperature_; }
  int max_absolute_temperature() { return max_absolute_temperature_; }
  int min_absolute_temperature() { return min_absolute_temperature_; }
  int max_absolute_annual_rainfall() { return max_absolute_annual_rainfall_; }
  int min_absolute_annual_rainfall() { return min_absolute_annual_rainfall_;; }
  int max_optimal_annual_rainfall() { return max_optimal_annual_rainfall_; }
  int min_optimal_annual_rainfall() { return min_optimal_annual_rainfall_; }
  bool cultivar() { return cultivar_; }

 private:
    // the name of the single species or single cultivar
    std::string name_;
 
    int* gdd_thresholds_;
    
    int base_temp_; // base temperature for Growing Degree Day calculations

    int max_optimal_temperature_;  // max optimal temperature requirement for plant type

    int min_optimal_temperature_;  // min optimal temperature requirement for plant type

    int max_absolute_temperature_;  // max absolute temperature requirement for plant type

    int min_absolute_temperature_;  // min absolute temperature requirement for plant type

    int max_absolute_annual_rainfall_;  // max absolute rainfall requirement for plant type

    int min_absolute_annual_rainfall_;  // min absolute rainfall requirement for plant type

    int max_optimal_annual_rainfall_;  // max optimal rainfall requirement for plant type

    int min_optimal_annual_rainfall_;  // max optimal rainfall requirement for plant type

    bool cultivar_;  //  specify whether this is a single species or single cultivar.

};

#endif  // AGROECOLOGY_COMMON_PLANT_TYPE_H_
