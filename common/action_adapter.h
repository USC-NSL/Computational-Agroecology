//Copyright 2019
#ifndef COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H
#define COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H

#include "plant_type.h"
#include "corn.h"
#include "bean.h"
#include "squash.h"

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

    ActionType type_;
private:
    int x_;
    int y_;
};

// This will allow agent to add crop to certain position in the terrain
class AddCrop : public ActionAdapter {
public:
    AddCrop(PlantType plantType, int x, int y):
      ActionAdapter(ADD_CROP, x, y) {
        plantType_ = plantType;
    }

private:
    PlantType plantType_;
};

// This will allow agent to remove crop in certain position
class RemoveCrop : public ActionAdapter {
public:
    RemoveCrop(int x, int y) : ActionAdapter(REMOVE_CROP, x, y) {}

};

// This will allow agent to add water to certain position
class AddWater : public ActionAdapter {
public:
    AddWater(int x, int y, int amount) : ActionAdapter(ADD_WATER, x, y) {
        amount_ = amount;
    }

private:
    int amount_;
};

// This will allow agent to harvest crop in certain position
class HarvestCrop : public ActionAdapter {
public:
    HarvestCrop(int x, int y) : ActionAdapter(HARVEST_CROP, x, y) {}
};

#endif //COMPUTATIONAL_AGROECOLOGY_ACTION_ADAPTER_H
