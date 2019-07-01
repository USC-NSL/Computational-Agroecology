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

class AddWaterTest : public ::testing::Test {
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
        ASSERT_NE(std::nullopt, terrain.tiles()[i][j].plant);
        EXPECT_EQ(kCornTypeName, terrain.tiles()[i][j].plant->type_name);
      } else {
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
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
        ASSERT_NE(std::nullopt, terrain.tiles()[i][j].plant);
        EXPECT_EQ(kCornTypeName, terrain.tiles()[i][j].plant->type_name);
      } else {
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
      }
    }
  }
}

TEST_F(AddTest, OperatorTest) {
  crop::Add lhs(applied_range, time, duration, kCornTypeName);
  EXPECT_TRUE(lhs == lhs);

  crop::Add rhs(applied_range, time, duration, "Bean");
  EXPECT_FALSE(lhs == rhs);
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
        ASSERT_NE(std::nullopt, terrain.tiles()[i][j].plant);
        EXPECT_EQ(kCornTypeName, terrain.tiles()[i][j].plant->type_name);
      } else {
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
      }
    }
  }

  crop::Remove remove_action(applied_range.front(), time, duration);

  remove_action.Execute(&terrain);
  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
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
        ASSERT_NE(std::nullopt, terrain.tiles()[i][j].plant);
        EXPECT_EQ(kCornTypeName, terrain.tiles()[i][j].plant->type_name);
      } else {
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
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
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
      }
    }
  }
}

TEST_F(RemoveTest, OperatorTest) {
  crop::Remove lhs(applied_range, time, duration);
  EXPECT_TRUE(lhs == lhs);

  crop::Remove rhs(applied_range, time, std::chrono::hours(10));
  EXPECT_FALSE(lhs == rhs);
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
  ASSERT_TRUE(terrain->tiles().get(applied_range[0]).plant.has_value());
  environment::Plant& plant = *(terrain->tiles().get(applied_range[0]).plant);
  plant.produce = 5;

  crop::Harvest harvest_action(applied_range.front(), time, duration);
  harvest_action.Execute(terrain);
  EXPECT_EQ(5, terrain->yield());
}

TEST_F(HarvestTest, OperatorTest) {
  crop::Harvest lhs(applied_range, time, duration);
  EXPECT_TRUE(lhs == lhs);

  crop::Harvest rhs(applied_range, time, std::chrono::hours(10));
  EXPECT_FALSE(lhs == rhs);
}

TEST_F(AddWaterTest, ConstrcutorTest_1) {
  crop::Water action(applied_range.front(), time, duration, 10.0);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(AddWaterTest, ConstrcutorTest_2) {
  crop::Water action(applied_range, time, duration, 10.0);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_TRUE(action.cost.empty());
}

TEST_F(AddWaterTest, ConstrcutorTest_3) {
  crop::Water action(applied_range.front(), time, duration, 10.0, cost);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

TEST_F(AddWaterTest, ConstrcutorTest_4) {
  crop::Water action(applied_range, time, duration, 10.0, cost);

  EXPECT_EQ(applied_range, action.applied_range);
  EXPECT_EQ(time, action.start_time);
  EXPECT_EQ(duration, action.duration);
  EXPECT_EQ(cost, action.cost);
}

// Adds water to a range of plants
TEST_F(AddWaterTest, AddToRange1) {
  crop::Add action(applied_range.front(), time, duration, kCornTypeName);
  Terrain terrain(kNumberOfRange);
  action.Execute(&terrain);
  // add water to terrain
  crop::Water water_action(applied_range.front(), time, duration, 15.0);
  water_action.Execute(&terrain);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        ASSERT_NE(std::nullopt, terrain.tiles()[i][j].plant);
        EXPECT_EQ(15.0, terrain.tiles()[i][j].soil.water_content);
      } else {
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
      }
    }
  }
}

// Adds water to a range of plants twice
TEST_F(AddWaterTest, AddToRange2) {
  crop::Add action(applied_range.front(), time, duration, kCornTypeName);
  Terrain terrain(kNumberOfRange);
  action.Execute(&terrain);
  // add water to terrain
  crop::Water water_action(applied_range.front(), time, duration, 15.0);
  water_action.Execute(&terrain);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        ASSERT_NE(std::nullopt, terrain.tiles()[i][j].plant);
        EXPECT_EQ(15.0, terrain.tiles()[i][j].soil.water_content);
      } else {
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
      }
    }
  }
  // add same amount of water to the range again
  water_action.Execute(&terrain);

  for (size_t i = 0; i < terrain.width(); ++i) {
    for (size_t j = 0; j < terrain.length(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        ASSERT_NE(std::nullopt, terrain.tiles()[i][j].plant);
        EXPECT_EQ(30.0, terrain.tiles()[i][j].soil.water_content);
      } else {
        EXPECT_EQ(std::nullopt, terrain.tiles()[i][j].plant);
      }
    }
  }
}

TEST_F(AddWaterTest, OperatorTest) {
  crop::Water lhs(applied_range, time, duration, 10.0);
  EXPECT_TRUE(lhs == lhs);

  crop::Water rhs(applied_range, time, std::chrono::hours(10), 10.0);
  EXPECT_FALSE(lhs == rhs);

  crop::Water rhs_2(applied_range, time, duration, 5.0);
  EXPECT_FALSE(lhs == rhs_2);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}