#include <chrono>
#include <vector>

#include <gtest/gtest.h>

#include "simulators/actions/crop.h"
#include "terrain.h"

using namespace simulator;
using namespace simulator::action;
using namespace environment;

const size_t kNumberOfRange = 5;

class AddTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (size_t i = 0; i < kNumberOfRange; ++i) {
      applied_range.push_back(Coordinate(i, i));
    }

    time = std::chrono::system_clock::now();
    duration = std::chrono::minutes(10);

    cost.push_back(
        std::make_pair<resource_type, size_t>(resource_type::MONEY, 20));
    cost.push_back(
        std::make_pair<resource_type, size_t>(resource_type::TIME, 40));
  }

  std::vector<Coordinate> applied_range;
  std::chrono::system_clock::time_point time;
  std::chrono::duration<int> duration;
  std::vector<std::pair<resource_type, size_t>> cost;
};

TEST_F(AddTest, ConstrcutorTest_1) {
  crop::Add action(applied_range.front(), time, duration, plant_type::CORN);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(AddTest, ConstrcutorTest_2) {
  crop::Add action(applied_range, time, duration, plant_type::CORN);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(AddTest, ConstrcutorTest_3) {
  crop::Add action(applied_range.front(), time, duration, plant_type::CORN,
                   cost);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(AddTest, ConstrcutorTest_4) {
  crop::Add action(applied_range, time, duration, plant_type::CORN, cost);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(AddTest, ExecuteTest_1) {
  crop::Add action(applied_range.front(), time, duration, plant_type::CORN);
  Terrain terrain(kNumberOfRange);

  ActionList::CROP_ADD_execute(&terrain, &action);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        EXPECT_NE(nullptr, terrain.tiles()[i][j].plant);
        EXPECT_EQ(plant_type::CORN, terrain.tiles()[i][j].plant->type);
      } else {
        EXPECT_EQ(nullptr, terrain.tiles()[i][j].plant);
      }
    }
  }
}

TEST_F(AddTest, ExecuteTest_2) {
  crop::Add action(applied_range, time, duration, plant_type::CORN);
  Terrain terrain(kNumberOfRange);

  ActionList::CROP_ADD_execute(&terrain, &action);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      // if the (i, j) is in the `applied_range`, we should see the effect of
      // this action
      if (std::find(applied_range.begin(), applied_range.end(),
                    Coordinate(i, j)) != applied_range.end()) {
        EXPECT_NE(nullptr, terrain.tiles()[i][j].plant);
        EXPECT_EQ(plant_type::CORN, terrain.tiles()[i][j].plant->type);
      } else {
        EXPECT_EQ(nullptr, terrain.tiles()[i][j].plant);
      }
    }
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}