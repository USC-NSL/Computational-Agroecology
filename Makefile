CXX := g++
CXXFLAGS := -std=c++17
OPENGLLIBS := -lGL -lglut -lGLEW

INCLUDES := -I ./

.DEFAULT_GOAL := all

# TODO: migrate the rules for "agent_server" to here

# components in environment
ENVIRONMENT_PATH := ./environment
MAIN_OBJ := $(ENVIRONMENT_PATH)/main.o
ENVIRONMENT_OBJ := $(ENVIRONMENT_PATH)/climate.o \
	$(ENVIRONMENT_PATH)/config.o \
	$(ENVIRONMENT_PATH)/environment.o \
	$(ENVIRONMENT_PATH)/location.o \
	$(ENVIRONMENT_PATH)/plant_builder.o \
	$(ENVIRONMENT_PATH)/plant.o \
	$(ENVIRONMENT_PATH)/soil.o \
	$(ENVIRONMENT_PATH)/sun_info.o \
	$(ENVIRONMENT_PATH)/terrain.o \
	$(ENVIRONMENT_PATH)/weather.o

# components in agent
AGENT_PATH := ./agent
AGENT_OBJ := $(AGENT_PATH)/agent.o $(AGENT_PATH)/q_learning.o
ACTION_PATH := $(AGENT_PATH)/actions
ACTION_OBJ := $(ACTION_PATH)/action.o $(ACTION_PATH)/crop.o

# components in environment/plants
PLANTS_PATH := $(ENVIRONMENT_PATH)/plants
PLANTS_OBJ := $(PLANTS_PATH)/bean.o

# components in environment/simulators (should be removed)
SIMULATOR_PATH := $(ENVIRONMENT_PATH)/simulators
PHOTON_SIMULATOR_PATH := $(SIMULATOR_PATH)/photon_simulator
PHOTON_SIMULATOR_LOADER_PATH := $(PHOTON_SIMULATOR_PATH)/loader
PHOTON_SIMULATOR_LOADER_OBJ := $(PHOTON_SIMULATOR_LOADER_PATH)/tiny_obj_loader.o
PHOTON_SIMULATOR_MODEL_PATH := $(PHOTON_SIMULATOR_PATH)/model
PHOTON_SIMULATOR_MODEL_OBJ := $(PHOTON_SIMULATOR_MODEL_PATH)/face.o\
	$(PHOTON_SIMULATOR_MODEL_PATH)/material.o\
	$(PHOTON_SIMULATOR_MODEL_PATH)/mesh.o\
	$(PHOTON_SIMULATOR_MODEL_PATH)/model.o\
	$(PHOTON_SIMULATOR_LOADER_OBJ)
PHOTON_SIMULATOR_OBJ := $(PHOTON_SIMULATOR_MODEL_OBJ)
SIMULATOR_OBJ := $(PHOTON_SIMULATOR_MODEL_OBJ)

ALL_OBJ_WITHOUT_MAIN := \
	$(ENVIRONMENT_OBJ) \
	$(AGENT_OBJ) \
	$(ACTION_OBJ) \
	$(PLANTS_OBJ) \
	$(SIMULATOR_OBJ)
ALL_OBJ := $(MAIN_OBJ) $(ALL_OBJ_WITHOUT_MAIN)

# unit tests
TEST_PATH := ./tests

TEST_AGENT_PATH := $(TEST_PATH)/agent
TEST_AGENT_OBJ := $(TEST_AGENT_PATH)/agent_test.o \
	$(TEST_AGENT_PATH)/qlearning_test.o
TEST_AGENT := $(TEST_AGENT_PATH)/agent_test \
	$(TEST_AGENT_PATH)/qlearning_test

TEST_AGENT_ACTIONS_PATH := $(TEST_AGENT_PATH)/actions
TEST_AGENT_ACTIONS_OBJ := $(TEST_AGENT_ACTIONS_PATH)/crop_test.o
TEST_AGENT_ACTIONS := $(TEST_AGENT_ACTIONS_PATH)/crop_test

TEST_ENVIRONMENT_PATH := $(TEST_PATH)/environment
TEST_ENVIRONMENT_OBJ := $(TEST_ENVIRONMENT_PATH)/climate_test.o \
	$(TEST_ENVIRONMENT_PATH)/config_test.o \
	$(TEST_ENVIRONMENT_PATH)/environment_test.o \
	$(TEST_ENVIRONMENT_PATH)/location_test.o \
	$(TEST_ENVIRONMENT_PATH)/soil_test.o \
	$(TEST_ENVIRONMENT_PATH)/terrain_test.o \
	$(TEST_ENVIRONMENT_PATH)/utility_test.o \
	$(TEST_ENVIRONMENT_PATH)/weather_test.o
TEST_ENVIRONMENT := $(TEST_ENVIRONMENT_PATH)/climate_test \
	$(TEST_ENVIRONMENT_PATH)/config_test \
	$(TEST_ENVIRONMENT_PATH)/environment_test \
	$(TEST_ENVIRONMENT_PATH)/location_test \
	$(TEST_ENVIRONMENT_PATH)/soil_test \
	$(TEST_ENVIRONMENT_PATH)/terrain_test \
	$(TEST_ENVIRONMENT_PATH)/utility_test \
	$(TEST_ENVIRONMENT_PATH)/weather_test

TEST_ENVIRONMENT_PLANTS_PATH := $(TEST_ENVIRONMENT_PATH)/plants
TEST_ENVIRONMENT_PLANTS_OBJ := $(TEST_ENVIRONMENT_PLANTS_PATH)/bean_test.o
TEST_ENVIRONMENT_PLANTS := $(TEST_ENVIRONMENT_PLANTS_PATH)/bean_test

TEST_SIMULATORS_PATH := $(TEST_PATH)/simulators
TEST_PHOTON_SIMULATOR_PATH := $(TEST_SIMULATORS_PATH)/photon_simulator
TEST_PHOTON_SIMULATOR_MODEL_OBJ := $(TEST_PHOTON_SIMULATOR_PATH)/photon_simulator_model_test.o
TEST_PHOTON_SIMULATOR_MODEL := $(TEST_PHOTON_SIMULATOR_PATH)/photon_simulator_model_test

TEST_OBJ := $(TEST_AGENT_OBJ) \
	$(TEST_AGENT_ACTIONS_OBJ) \
	$(TEST_ENVIRONMENT_OBJ) \
	$(TEST_ENVIRONMENT_PLANTS_OBJ) \
	$(TEST_PHOTON_SIMULATOR_MODEL_OBJ)
TEST_ALL := $(TEST_AGENT) \
	$(TEST_AGENT_ACTIONS) \
	$(TEST_ENVIRONMENT) \
	$(TEST_ENVIRONMENT_PLANTS) \
	$(TEST_PHOTON_SIMULATOR_MODEL)
TESTFLAGS := -Igtest/include
TESTLD := -lgtest -lpthread

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%_test.o: %_test.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TESTFLAGS) -c $< -o $@

%_test: %_test.o $(ALL_OBJ_WITHOUT_MAIN)
	$(CXX) $< $(ALL_OBJ_WITHOUT_MAIN) -o $@ $(TESTLD) $(OPENGLLIBS)

all: $(ALL_OBJ)
	$(CXX) $(ALL_OBJ) -o main $(OPENGLLIBS)

environment: $(ENVIRONMENT_OBJ)
agent: $(AGENT_OBJ)
action: $(ACTION_OBJ)
plants: $(PLANTS_OBJ)

all_test: $(TEST_ALL)

all_test_run: $(TEST_ALL)
	@for var in $(TEST_ALL); do \
		echo $$var; \
		$$var; \
	done

clean:
	rm -f $(ALL_OBJ) $(TEST_OBJ) $(TEST_ALL)
	rm -f main
