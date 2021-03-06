#include <algorithm>
#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "agent/actions/crop.h"
#include "agent/resource.h"
#include "environment/terrain.h"

using namespace agent::action;
using namespace config;
using namespace environment;

const size_t kNumberOfRange = 5;

class AddTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (size_t i = 0; i < kNumberOfRange; ++i) {
      applied_range.push_back(Coordinate(i, i));
    }

    cost[agent::ResourceType::MONEY] = 20;
    cost[agent::ResourceType::LABOR] = 40;

    Config dumb_config("place name", Location(100, 101, 201, 200));
    Climate dumb_climate(dumb_config);
    Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Meteorology dumb_meteorology(std::chrono::system_clock::now(),
                                 dumb_config.location,
                                 dumb_climate.climate_zone, dumb_weather);
    TerrainRawData dumb_terrain_raw_data(kNumberOfRange, 0);
    terrain.reset(new Terrain(dumb_terrain_raw_data, dumb_meteorology));
  }

  std::vector<Coordinate> applied_range;
  int64_t time_step = 0;
  int64_t duration = 1;
  agent::Resources cost;
  std::unique_ptr<Terrain> terrain;
};

class RemoveTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (size_t i = 0; i < kNumberOfRange; ++i) {
      applied_range.push_back(Coordinate(i, i));
    }

    cost[agent::ResourceType::MONEY] = 20;
    cost[agent::ResourceType::LABOR] = 40;

    Config dumb_config("place name", Location(100, 101, 201, 200));
    Climate dumb_climate(dumb_config);
    Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Meteorology dumb_meteorology(std::chrono::system_clock::now(),
                                 dumb_config.location,
                                 dumb_climate.climate_zone, dumb_weather);
    TerrainRawData dumb_terrain_raw_data(kNumberOfRange, 0);
    terrain.reset(new Terrain(dumb_terrain_raw_data, dumb_meteorology));
  }

  std::vector<Coordinate> applied_range;
  int64_t time_step = 0;
  int64_t duration = 1;
  agent::Resources cost;
  std::unique_ptr<Terrain> terrain;
};

class HarvestTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (size_t i = 0; i < kNumberOfRange; ++i) {
      applied_range.push_back(Coordinate(i, i));
    }

    cost[agent::ResourceType::MONEY] = 20;
    cost[agent::ResourceType::LABOR] = 40;

    Config dumb_config("place name", Location(100, 101, 201, 200));
    Climate dumb_climate(dumb_config);
    Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Meteorology dumb_meteorology(std::chrono::system_clock::now(),
                                 dumb_config.location,
                                 dumb_climate.climate_zone, dumb_weather);
    TerrainRawData dumb_terrain_raw_data(kNumberOfRange, 0);
    terrain.reset(new Terrain(dumb_terrain_raw_data, dumb_meteorology));
  }

  std::vector<Coordinate> applied_range;
  int64_t time_step = 0;
  int64_t duration = 1;
  agent::Resources cost;
  std::unique_ptr<Terrain> terrain;
};

class AddWaterTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (size_t i = 0; i < kNumberOfRange; ++i) {
      applied_range.push_back(Coordinate(i, i));
    }

    cost[agent::ResourceType::MONEY] = 20;
    cost[agent::ResourceType::LABOR] = 40;

    Config dumb_config("place name", Location(100, 101, 201, 200));
    Climate dumb_climate(dumb_config);
    Weather dumb_weather(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Meteorology dumb_meteorology(std::chrono::system_clock::now(),
                                 dumb_config.location,
                                 dumb_climate.climate_zone, dumb_weather);
    TerrainRawData dumb_terrain_raw_data(kNumberOfRange, 0);
    terrain.reset(new Terrain(dumb_terrain_raw_data, dumb_meteorology));
  }

  std::vector<Coordinate> applied_range;
  int64_t time_step = 0;
  int64_t duration = 1;
  agent::Resources cost;
  std::unique_ptr<Terrain> terrain;
};

const std::string kBeanTypeName = "bean";

TEST_F(AddTest, ConstrcutorTest_1) {
  crop::Add action(applied_range.front(), time_step, duration, kBeanTypeName);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_TRUE(action.cost().empty());
}

TEST_F(AddTest, ConstrcutorTest_2) {
  crop::Add action(applied_range, time_step, duration, kBeanTypeName);

  EXPECT_EQ(applied_range, action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_TRUE(action.cost().empty());
}

TEST_F(AddTest, ConstrcutorTest_3) {
  crop::Add action(applied_range.front(), time_step, duration, cost,
                   kBeanTypeName);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_EQ(cost, action.cost());
}

TEST_F(AddTest, ConstrcutorTest_4) {
  crop::Add action(applied_range, time_step, duration, cost, kBeanTypeName);

  EXPECT_EQ(applied_range, action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_EQ(cost, action.cost());
}

TEST_F(AddTest, ExecuteTest_1) {
  crop::Add action(applied_range.front(), time_step, duration, kBeanTypeName);

  action.Execute(terrain.get());

  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      const Plant *plant = terrain->plant_container()[Coordinate(i, j)];
      if (Coordinate(i, j) == applied_range.front()) {
        ASSERT_NE(nullptr, plant);
        EXPECT_EQ(kBeanTypeName, plant->name());
      } else {
        EXPECT_EQ(nullptr, plant);
      }
    }
  }
}

TEST_F(AddTest, ExecuteTest_2) {
  crop::Add action(applied_range, time_step, duration, kBeanTypeName);

  action.Execute(terrain.get());

  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      // if the (i, j) is in the `applied_range`, we should see the effect of
      // this action
      const Plant *plant = terrain->plant_container()[Coordinate(i, j)];
      if (std::find(applied_range.begin(), applied_range.end(),
                    Coordinate(i, j)) != applied_range.end()) {
        ASSERT_NE(nullptr, plant);
        EXPECT_EQ(kBeanTypeName, plant->name());
      } else {
        EXPECT_EQ(nullptr, plant);
      }
    }
  }
}

TEST_F(AddTest, OperatorTest) {
  crop::Add lhs(applied_range, time_step, duration, kBeanTypeName);
  EXPECT_TRUE(lhs == lhs);

  crop::Add rhs(applied_range, time_step, duration, "corn");
  EXPECT_FALSE(lhs == rhs);
}

TEST_F(RemoveTest, ConstrcutorTest_1) {
  crop::Remove action(applied_range.front(), time_step, duration);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_TRUE(action.cost().empty());
}

TEST_F(RemoveTest, ConstrcutorTest_2) {
  crop::Remove action(applied_range, time_step, duration);

  EXPECT_EQ(applied_range, action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_TRUE(action.cost().empty());
}

TEST_F(RemoveTest, ConstrcutorTest_3) {
  crop::Remove action(applied_range.front(), time_step, duration, cost);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_EQ(cost, action.cost());
}

TEST_F(RemoveTest, ConstrcutorTest_4) {
  crop::Remove action(applied_range, time_step, duration, cost);

  EXPECT_EQ(applied_range, action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_EQ(cost, action.cost());
}

TEST_F(RemoveTest, ExecuteTest_1) {
  crop::Add action(applied_range.front(), time_step, duration, kBeanTypeName);

  action.Execute(terrain.get());

  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      const Plant *plant = terrain->plant_container()[Coordinate(i, j)];
      if (Coordinate(i, j) == applied_range.front()) {
        ASSERT_NE(nullptr, plant);
        EXPECT_EQ(kBeanTypeName, plant->name());
      } else {
        EXPECT_EQ(nullptr, plant);
      }
    }
  }

  crop::Remove remove_action(applied_range.front(), time_step, duration);

  remove_action.Execute(terrain.get());
  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      if (Coordinate(i, j) == applied_range.front()) {
        EXPECT_EQ(nullptr, terrain->plant_container()[Coordinate(i, j)]);
      }
    }
  }
}

TEST_F(RemoveTest, ExecuteTest_2) {
  crop::Add action(applied_range, time_step, duration, kBeanTypeName);

  action.Execute(terrain.get());

  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      // if the (i, j) is in the `applied_range`, we should see the effect of
      // this action
      const Plant *plant = terrain->plant_container()[Coordinate(i, j)];
      if (std::find(applied_range.begin(), applied_range.end(),
                    Coordinate(i, j)) != applied_range.end()) {
        ASSERT_NE(nullptr, plant);
        EXPECT_EQ(kBeanTypeName, plant->name());
      } else {
        EXPECT_EQ(nullptr, plant);
      }
    }
  }

  crop::Remove remove_action(applied_range, time_step, duration);

  remove_action.Execute(terrain.get());
  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      // if the (i, j) is in the `applied_range`, we should see the effect of
      // this action
      if (std::find(applied_range.begin(), applied_range.end(),
                    Coordinate(i, j)) != applied_range.end()) {
        EXPECT_EQ(nullptr, terrain->plant_container()[Coordinate(i, j)]);
      }
    }
  }
}

TEST_F(RemoveTest, OperatorTest) {
  crop::Remove lhs(applied_range, time_step, duration);
  EXPECT_TRUE(lhs == lhs);

  crop::Remove rhs(applied_range, time_step, 10);
  EXPECT_FALSE(lhs == rhs);
}

TEST_F(HarvestTest, ConstrcutorTest_1) {
  crop::Harvest action(applied_range.front(), time_step, duration);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_TRUE(action.cost().empty());
}

TEST_F(HarvestTest, ConstrcutorTest_2) {
  crop::Harvest action(applied_range, time_step, duration);

  EXPECT_EQ(applied_range, action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_TRUE(action.cost().empty());
}

TEST_F(HarvestTest, ConstrcutorTest_3) {
  crop::Harvest action(applied_range.front(), time_step, duration, cost);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_EQ(cost, action.cost());
}

TEST_F(HarvestTest, ConstrcutorTest_4) {
  crop::Harvest action(applied_range, time_step, duration, cost);

  EXPECT_EQ(applied_range, action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_EQ(cost, action.cost());
}

TEST_F(HarvestTest, ExecuteTest_1) {
  // This could not be tested now since there is no function to make a plant
  // grow and even produce.
}

TEST_F(HarvestTest, OperatorTest) {
  crop::Harvest lhs(applied_range, time_step, duration);
  EXPECT_TRUE(lhs == lhs);

  crop::Harvest rhs(applied_range, time_step, 10);
  EXPECT_FALSE(lhs == rhs);
}

TEST_F(AddWaterTest, ConstrcutorTest_1) {
  crop::Water action(applied_range.front(), time_step, duration, 10.0);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_TRUE(action.cost().empty());
}

TEST_F(AddWaterTest, ConstrcutorTest_2) {
  crop::Water action(applied_range, time_step, duration, 10.0);

  EXPECT_EQ(applied_range, action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_TRUE(action.cost().empty());
}

TEST_F(AddWaterTest, ConstrcutorTest_3) {
  crop::Water action(applied_range.front(), time_step, duration, cost, 10.0);

  EXPECT_EQ(std::vector<Coordinate>(1, applied_range.front()),
            action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_EQ(cost, action.cost());
}

TEST_F(AddWaterTest, ConstrcutorTest_4) {
  crop::Water action(applied_range, time_step, duration, cost, 10.0);

  EXPECT_EQ(applied_range, action.applied_range());
  EXPECT_EQ(time_step, action.start_time_step());
  EXPECT_EQ(duration, action.duration());
  EXPECT_EQ(cost, action.cost());
}

// Adds water to a range of plants
TEST_F(AddWaterTest, AddToRange1) {
  crop::Add action(applied_range.front(), time_step, duration, kBeanTypeName);

  action.Execute(terrain.get());
  // add water to terrain
  crop::Water water_action(applied_range.front(), time_step, duration, 15.0);
  water_action.Execute(terrain.get());

  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      const Plant *plant = terrain->plant_container()[Coordinate(i, j)];
      if (Coordinate(i, j) == applied_range.front()) {
        ASSERT_NE(nullptr, plant);
        EXPECT_EQ(15.0, terrain->soil_container()[Coordinate(i, j)]
                            .water_content()
                            .water_amount_1);
      } else {
        EXPECT_EQ(nullptr, plant);
      }
    }
  }
}

// Adds water to a range of plants twice
TEST_F(AddWaterTest, AddToRange2) {
  crop::Add action(applied_range.front(), time_step, duration, kBeanTypeName);

  action.Execute(terrain.get());
  // add water to terrain
  crop::Water water_action(applied_range.front(), time_step, duration, 15.0);
  water_action.Execute(terrain.get());

  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      const Plant *plant = terrain->plant_container()[Coordinate(i, j)];
      if (Coordinate(i, j) == applied_range.front()) {
        ASSERT_NE(nullptr, plant);
        EXPECT_EQ(15.0, terrain->soil_container()[Coordinate(i, j)]
                            .water_content()
                            .water_amount_1);
      } else {
        EXPECT_EQ(nullptr, plant);
      }
    }
  }
  // add same amount of water to the range again
  water_action.Execute(terrain.get());

  for (size_t i = 0; i < terrain->size(); ++i) {
    for (size_t j = 0; j < terrain->size(); ++j) {
      const Plant *plant = terrain->plant_container()[Coordinate(i, j)];
      if (Coordinate(i, j) == applied_range.front()) {
        ASSERT_NE(nullptr, plant);
        EXPECT_EQ(30.0, terrain->soil_container()[Coordinate(i, j)]
                            .water_content()
                            .water_amount_1);
      } else {
        EXPECT_EQ(nullptr, plant);
      }
    }
  }
}

TEST_F(AddWaterTest, OperatorTest) {
  crop::Water lhs(applied_range, time_step, duration, 10.0);
  EXPECT_TRUE(lhs == lhs);

  crop::Water rhs(applied_range, time_step, 10, 10.0);
  EXPECT_FALSE(lhs == rhs);

  crop::Water rhs_2(applied_range, time_step, duration, 5.0);
  EXPECT_FALSE(lhs == rhs_2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
