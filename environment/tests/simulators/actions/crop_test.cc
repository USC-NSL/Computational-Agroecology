#include <algorithm>
#include <chrono>
#include <string>
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
        std::make_pair<ResourceType, size_t>(ResourceType::MONEY, 20));
    cost.push_back(
        std::make_pair<ResourceType, size_t>(ResourceType::LABOR, 40));
  }

  std::vector<Coordinate> applied_range;
  std::chrono::system_clock::time_point time;
  std::chrono::duration<int> duration;
  std::vector<std::pair<ResourceType, size_t>> cost;
};

class RemoveTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (size_t i = 0; i < kNumberOfRange; ++i) {
      applied_range.push_back(Coordinate(i, i));
    }

    time = std::chrono::system_clock::now();
    duration = std::chrono::minutes(10);

    cost.push_back(
        std::make_pair<ResourceType, size_t>(ResourceType::MONEY, 20));
    cost.push_back(
        std::make_pair<ResourceType, size_t>(ResourceType::LABOR, 40));
  }

  std::vector<Coordinate> applied_range;
  std::chrono::system_clock::time_point time;
  std::chrono::duration<int> duration;
  std::vector<std::pair<ResourceType, size_t>> cost;
};

class HarvestTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (size_t i = 0; i < kNumberOfRange; ++i) {
      applied_range.push_back(Coordinate(i, i));
    }

    time = std::chrono::system_clock::now();
    duration = std::chrono::minutes(10);

    cost.push_back(
        std::make_pair<ResourceType, size_t>(ResourceType::MONEY, 20));
    cost.push_back(
        std::make_pair<ResourceType, size_t>(ResourceType::LABOR, 40));
  }

  std::vector<Coordinate> applied_range;
  std::chrono::system_clock::time_point time;
  std::chrono::duration<int> duration;
  std::vector<std::pair<ResourceType, size_t>> cost;
};

const std::string kCornTypeName = "Corn";

TEST_F(AddTest, ConstrcutorTest_1) {
  crop::Add action(applied_range.front(), time, duration, kCornTypeName);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(AddTest, ConstrcutorTest_2) {
  crop::Add action(applied_range, time, duration, kCornTypeName);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(AddTest, ConstrcutorTest_3) {
  crop::Add action(applied_range.front(), time, duration, kCornTypeName, cost);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(AddTest, ConstrcutorTest_4) {
  crop::Add action(applied_range, time, duration, kCornTypeName, cost);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(AddTest, ExecuteTest_1) {
  crop::Add action(applied_range.front(), time, duration, kCornTypeName);
  Terrain terrain(kNumberOfRange);

  action.Execute(&terrain);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        EXPECT_NE(nullptr, terrain.tiles()[i][j].plant);
        EXPECT_EQ(kCornTypeName, terrain.tiles()[i][j].plant->type_name);
      } else {
        EXPECT_EQ(nullptr, terrain.tiles()[i][j].plant);
      }
    }
  }
}

TEST_F(AddTest, ExecuteTest_2) {
  crop::Add action(applied_range, time, duration, kCornTypeName);
  Terrain terrain(kNumberOfRange);

  action.Execute(&terrain);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      // if the (i, j) is in the `applied_range`, we should see the effect of
      // this action
      if (std::find(applied_range.begin(), applied_range.end(),
                    Coordinate(i, j)) != applied_range.end()) {
        EXPECT_NE(nullptr, terrain.tiles()[i][j].plant);
        EXPECT_EQ(kCornTypeName, terrain.tiles()[i][j].plant->type_name);
      } else {
        EXPECT_EQ(nullptr, terrain.tiles()[i][j].plant);
      }
    }
  }
}

TEST_F(RemoveTest, ConstrcutorTest_1) {
  crop::Remove action(applied_range.front(), time, duration);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(RemoveTest, ConstrcutorTest_2) {
  crop::Remove action(applied_range, time, duration);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(RemoveTest, ConstrcutorTest_3) {
  crop::Remove action(applied_range.front(), time, duration, cost);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(RemoveTest, ConstrcutorTest_4) {
  crop::Remove action(applied_range, time, duration, cost);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(RemoveTest, ExecuteTest_1) {
  crop::Add action(applied_range.front(), time, duration, kCornTypeName);
  Terrain terrain(kNumberOfRange);

  action.Execute(&terrain);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        EXPECT_NE(nullptr, terrain.tiles()[i][j].plant);
        EXPECT_EQ(kCornTypeName, terrain.tiles()[i][j].plant->type_name);
      } else {
        EXPECT_EQ(nullptr, terrain.tiles()[i][j].plant);
      }
    }
  }

  crop::Remove remove_action(applied_range.front(), time, duration);

  remove_action.Execute(&terrain);
  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        EXPECT_EQ(nullptr, terrain.tiles()[i][j].plant);
      }
    }
  }
}

TEST_F(RemoveTest, ExecuteTest_2) {
  crop::Add action(applied_range, time, duration, kCornTypeName);
  Terrain terrain(kNumberOfRange);

  action.Execute(&terrain);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      // if the (i, j) is in the `applied_range`, we should see the effect of
      // this action
      if (std::find(applied_range.begin(), applied_range.end(),
                    Coordinate(i, j)) != applied_range.end()) {
        EXPECT_NE(nullptr, terrain.tiles()[i][j].plant);
        EXPECT_EQ(kCornTypeName, terrain.tiles()[i][j].plant->type_name);
      } else {
        EXPECT_EQ(nullptr, terrain.tiles()[i][j].plant);
      }
    }
  }

  crop::Remove remove_action(applied_range, time, duration);

  remove_action.Execute(&terrain);
  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      // if the (i, j) is in the `applied_range`, we should see the effect of
      // this action
      if (std::find(applied_range.begin(), applied_range.end(),
                    Coordinate(i, j)) != applied_range.end()) {
        EXPECT_EQ(nullptr, terrain.tiles()[i][j].plant);
      }
    }
  }
}

TEST_F(HarvestTest, ConstrcutorTest_1) {
  crop::Harvest action(applied_range.front(), time, duration);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(HarvestTest, ConstrcutorTest_2) {
  crop::Harvest action(applied_range, time, duration);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(HarvestTest, ConstrcutorTest_3) {
  crop::Harvest action(applied_range.front(), time, duration, cost);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(HarvestTest, ConstrcutorTest_4) {
  crop::Harvest action(applied_range, time, duration, cost);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(HarvestTest, ExecuteTest_1) {
  crop::Add action(applied_range, time, duration, kCornTypeName);
  Terrain* terrain = new Terrain(kNumberOfRange);
  action.Execute(terrain);
  environment::Plant* plant = terrain->tiles().get(applied_range[0]).plant;
  plant->produce = 5;

  crop::Harvest harvest_action(applied_range.front(), time, duration);
  harvest_action.Execute(terrain);
  EXPECT_EQ(5, terrain->yield());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}