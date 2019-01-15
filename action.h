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
  ActionType type_;
  Location location_;
};

// TODO(barath): Add comment.
class AddCrop : public Action {
 public:
  AddCrop() : type_(ADD_CROP) {}

  virtual int GetDuration() override;
};

// TODO(barath): Add comment.
class RemoveCrop : public Action {
 public:
  RemoveCrop() : type_(REMOVE_CROP) {}

  virtual int GetDuration() override;
};

// TODO(barath): Add comment.
class ChangeTerrain : public Action {
 public:
  ChangeTerrain() : type_(CHANGE_TERRAIN) {}

  virtual int GetDuration() override;
};

// TODO(barath): Add comment.
class AddWater : public Action {
 public:
  AddWater() : type_(ADD_WATER) {}

  virtual int GetDuration() override;
};

#endif  // AGROECOLOGY_ACTION_H_
