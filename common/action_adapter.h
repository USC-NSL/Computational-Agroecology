// Copyright 2019
#ifndef COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H
#define COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H

#include "../sim/terrain.h"
#include "bean.h"
#include "corn.h"
#include "plant_type.h"
#include "squash.h"
#include <vector>

// Represents activities that can be done in a farm.
enum ActionType { ADD_CROP, REMOVE_CROP, ADD_WATER, HARVEST_CROP };

// Describes an action to be taken on a piece of land.
class ActionAdapter {
public:
  // Returns the type of this action.
  ActionType type() const { return type_; }

  ActionAdapter(ActionType type, int x = 0, int y = 0,
                int time_elapse_to_perform = 0, int duration = 0) {
    type_ = type;
    x_ = x;
    y_ = y;
    time_elapse_to_perform_ = time_elapse_to_perform;
    duration_ = duration;
  }
  // Return if the action perform successfully
  virtual bool perform_action(Terrain *terrain, std::vector<PlantType> plants) {
  }

  ActionType type_;            // the type of action got performed
  int x_;                      // the coordination of the action got performed
  int y_;                      // the coordination of the action got performed
  int time_elapse_to_perform_; // the elapse of time before action get performed
  int duration_;               // the duration of each action
};

// This will allow agent to add crop to certain position in the terrain
class AddCrop : public ActionAdapter {
public:
  AddCrop(PlantType *plantType, int x, int y) : ActionAdapter(ADD_CROP, x, y) {
    plantType_ = plantType;
  }

  virtual bool perform_action(Terrain *terrain, std::vector<PlantType> plants) {
    if ((*terrain).tiles_[x_][y_].occupied) {
      return false;
    }
    (*terrain).tiles_[x_][y_].occupied = true;
    plants.push_back(*plantType_);
    (*terrain).tiles_[x_][y_].plant = plantType_;
    return true;
  }

private:
  PlantType *plantType_;
};

// This will allow agent to remove crop in certain position
class RemoveCrop : public ActionAdapter {
public:
  RemoveCrop(int x, int y) : ActionAdapter(REMOVE_CROP, x, y) {}
  virtual bool perform_action(Terrain *terrain, std::vector<PlantType> plants) {
    if (!(*terrain).tiles_[x_][y_].occupied) {
      return false;
    }
    (*terrain).tiles_[x_][y_].occupied = false;
    (*terrain).tiles_[x_][y_].plant = NULL;
    return true;
  }
};

// This will allow agent to add water to certain position
class AddWater : public ActionAdapter {
public:
  AddWater(int x, int y, int amount) : ActionAdapter(ADD_WATER, x, y) {
    amount_ = amount;
  }
  virtual bool perform_action(Terrain *terrain, std::vector<PlantType> plants) {
    (*terrain).tiles_[x_][y_].soil->addWater(amount_);
    return true;
  }

private:
  int amount_;
};

// This will allow agent to harvest crop in certain position
class HarvestCrop : public ActionAdapter {
public:
  HarvestCrop(int x, int y) : ActionAdapter(HARVEST_CROP, x, y) {}
  virtual bool perform_action(Terrain terrain, std::vector<PlantType> plants) {}
};

#endif // COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H
