CXX := g++
CXXFLAGS := -std=c++17

SRC_DIR := ./src

INCLUDES := -I $(SRC_DIR)

MAIN_OBJ := $(SRC_DIR)/main.o

ENVIRONMENT_PATH := $(SRC_DIR)/environment
ENVIRONMENT_OBJ := $(ENVIRONMENT_PATH)/climate.o $(ENVIRONMENT_PATH)/config.o $(ENVIRONMENT_PATH)/environment.o $(ENVIRONMENT_PATH)/location.o $(ENVIRONMENT_PATH)/plant.o $(ENVIRONMENT_PATH)/soil.o $(ENVIRONMENT_PATH)/terrain.o $(ENVIRONMENT_PATH)/weather.o

AGENT_PATH := $(SRC_DIR)/agent
AGENT_OBJ := $(AGENT_PATH)/agent.o

PLANT_TYPE_PATH := $(ENVIRONMENT_PATH)/plant_types
PLANT_TYPE_OBJ := $(PLANT_TYPE_PATH)/plant_type.o $(PLANT_TYPE_PATH)/bean.o $(PLANT_TYPE_PATH)/corn.o $(PLANT_TYPE_PATH)/squash.o

SIMULATOR_PATH := $(SRC_DIR)/simulators
ACTION_PATH := $(SRC_DIR)/simulators/actions
SIMULATOR_OBJ := $(SIMULATOR_PATH)/main_simulator.o  $(SIMULATOR_PATH)/sun_simulator.o $(ACTION_PATH)/action.o $(ACTION_PATH)/crop.o

ALL_OBJ_WITHOUT_MAIN := $(ENVIRONMENT_OBJ) $(AGENT_OBJ) $(PLANT_TYPE_OBJ) $(SIMULATOR_OBJ)
ALL_OBJ := $(MAIN_OBJ) $(ALL_OBJ_WITHOUT_MAIN)

TEST_PATH := ./tests

TEST_ENVIRONMENT_PATH := $(TEST_PATH)/environment
TEST_ENVIRONMENT_OBJ := $(TEST_ENVIRONMENT_PATH)/climate_test.o $(TEST_ENVIRONMENT_PATH)/config_test.o $(TEST_ENVIRONMENT_PATH)/environment_test.o $(TEST_ENVIRONMENT_PATH)/location_test.o $(TEST_ENVIRONMENT_PATH)/plant_test.o $(TEST_ENVIRONMENT_PATH)/soil_test.o $(TEST_ENVIRONMENT_PATH)/terrain_test.o $(TEST_ENVIRONMENT_PATH)/weather_test.o
TEST_ENVIRONMENT := $(TEST_ENVIRONMENT_PATH)/climate_test $(TEST_ENVIRONMENT_PATH)/config_test $(TEST_ENVIRONMENT_PATH)/environment_test $(TEST_ENVIRONMENT_PATH)/location_test $(TEST_ENVIRONMENT_PATH)/plant_test $(TEST_ENVIRONMENT_PATH)/soil_test $(TEST_ENVIRONMENT_PATH)/terrain_test $(TEST_ENVIRONMENT_PATH)/weather_test

TEST_AGENT_PATH := $(TEST_PATH)/agent
TEST_AGENT_OBJ := $(TEST_AGENT_PATH)/agent_test.o
TEST_AGENT := $(TEST_AGENT_PATH)/agent_test

TEST_PLANT_TYPES_PATH := $(TEST_ENVIRONMENT_PATH)/plant_types
TEST_PLANT_TYPES_OBJ := $(TEST_PLANT_TYPES_PATH)/bean_test.o $(TEST_PLANT_TYPES_PATH)/corn_test.o $(TEST_PLANT_TYPES_PATH)/squash_test.o
TEST_PLANT_TYPES := $(TEST_PLANT_TYPES_PATH)/bean_test $(TEST_PLANT_TYPES_PATH)/corn_test $(TEST_PLANT_TYPES_PATH)/squash_test

TEST_SIMULATORS_PATH := $(TEST_PATH)/simulators
TEST_SIMULATORS_OBJ := $(TEST_SIMULATORS_PATH)/main_simulator_test.o $(TEST_SIMULATORS_PATH)/sun_simulator_test.o
TEST_SIMULATORS := $(TEST_SIMULATORS_PATH)/main_simulator_test $(TEST_SIMULATORS_PATH)/sun_simulator_test

TEST_SIMULATORS_ACTIONS_PATH := $(TEST_SIMULATORS_PATH)/actions
TEST_SIMULATORS_ACTIONS_OBJ := $(TEST_SIMULATORS_ACTIONS_PATH)/crop_test.o
TEST_SIMULATORS_ACTIONS := $(TEST_SIMULATORS_ACTIONS_PATH)/crop_test

TEST_UTIL_PATH := $(TEST_PATH)/util
TEST_UTIL_OBJ := $(TEST_UTIL_PATH)/max_min_pair_test.o
TEST_UTIL := $(TEST_UTIL_PATH)/max_min_pair_test

TEST_OBJ := $(TEST_ENVIRONMENT_OBJ) $(TEST_AGENT_OBJ) $(TEST_PLANT_TYPES_OBJ) $(TEST_SIMULATORS_OBJ) $(TEST_SIMULATORS_ACTIONS_OBJ) $(TEST_UTIL_OBJ)
TEST_ALL := $(TEST_ENVIRONMENT) $(TEST_AGENT) $(TEST_PLANT_TYPES) $(TEST_SIMULATORS) $(TEST_SIMULATORS_ACTIONS) $(TEST_UTIL)
TESTFLAGS := -Igtest/include
TESTLD := -lgtest -lpthread

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%_test.o: %_test.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TESTFLAGS) -c $< -o $@

%_test: %_test.o $(ALL_OBJ_WITHOUT_MAIN)
	$(CXX) $< $(ALL_OBJ_WITHOUT_MAIN) -o $@ $(TESTLD)

all: $(ALL_OBJ)
	$(CXX) $(ALL_OBJ) -o dummy_main

all_test: $(TEST_ALL)

all_test_run: $(TEST_ALL)
	@for var in $(TEST_ALL); do \
		echo $$var; \
		$$var; \
	done

clean:
	rm -f $(ALL_OBJ) $(TEST_OBJ) $(TEST_ALL)
	rm -f dummy_main
