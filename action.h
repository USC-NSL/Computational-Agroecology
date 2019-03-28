#ifndef AGROECOLOGY_ACTION_H_
#define AGROECOLOGY_ACTION_H_

// Represents activities that can be done in a farm.
enum ActionType {
  ADD_CROP,
  REMOVE_CROP,
  CHANGE_TERRAIN,
  ADD_WATER
};

// Describes an action to be taken on a piece of land.
class Action {
  // Returns the duration in ticks of this specific action.
  virtual int GetDuration() = 0;
 
  // Returns the type of this action.
  ActionType type() const {
    return type_;
  }

 protected:

  explicit Action(ActionType type) {
    type_ = type;
  }

  ActionType type_;

  //TODO: Will this be refactored into landscape?
  //Location location_;
};

// TODO(barath): Add comment.
class AddCrop : public Action {
 public:
  AddCrop() : Action(ADD_CROP) {}

  int GetDuration() override;
};

// TODO(barath): Add comment.
class RemoveCrop : public Action {
 public:
  RemoveCrop() : Action(REMOVE_CROP) {}

  int GetDuration() override;
};

// TODO(barath): Add comment.
class ChangeTerrain : public Action {
 public:
  ChangeTerrain() : Action(CHANGE_TERRAIN) {}

  int GetDuration() override;
};

// TODO(barath): Add comment.
class AddWater : public Action {
 public:
  AddWater() : Action(ADD_WATER) {}

  int GetDuration() override;
};

#endif  // AGROECOLOGY_ACTION_H_
