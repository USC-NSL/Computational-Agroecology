//Copyright 2019
#ifndef COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H
#define COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H

#include "plant_type.h"
#include "corn.h"
#include "bean.h"
#include "squash.h"
#include <vector>
#include "../sim/terrain.h"

// Represents activities that can be done in a farm.
enum ActionType {
    ADD_CROP,
    REMOVE_CROP,
    ADD_WATER,
    HARVEST_CROP
};

// Describes an action to be taken on a piece of land.
class ActionAdapter {
public:

    // Returns the type of this action.
    ActionType type() const {
        return type_;
    }

    ActionAdapter(ActionType type, int x = 0, int y = 0)
    {
        type_ = type,
        x_ = x;
        y_ = y;
    }
    //Return if the action perform successfully
    virtual bool perform_action(Terrain *terrain, std::vector<PlantType> plants) {
    }
    ActionType type_;
    int x_;
    int y_;
};

// This will allow agent to add crop to certain position in the terrain
class AddCrop : public ActionAdapter {
public:
    AddCrop(PlantType *plantType, int x, int y):
      ActionAdapter(ADD_CROP, x, y) {
        plantType_ = plantType;
    }

    virtual bool perform_action(Terrain *terrain, std::vector<PlantType> plants) {
        if ((*terrain).tiles_[x_][y_].occupied)
            { return false; }
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
        if (!(*terrain).tiles_[x_][y_].occupied)
            { return false; }
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
   virtual  bool perform_action(Terrain terrain, std::vector<PlantType> plants) {

    }
};

#endif //COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H
