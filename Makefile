CXX := g++
CXXFLAGS := -std=c++17
OPENGLLIBS := -lGL -lglut -lGLEW

THIRD_PARTY_PATH := ./thirdparty

INCLUDES := -I ./
INCLUDES += -I $(THIRD_PARTY_PATH)
INCLUDES += -I $(THIRD_PARTY_PATH)/Optimized-Photon-Mapping/src

.DEFAULT_GOAL := all

# TODO: migrate the rules for "agent_server" to here

MAIN_NAME := ./environment/main

# components in environment
ENVIRONMENT_PATH := ./environment
ENVIRONMENT_OBJ := $(ENVIRONMENT_PATH)/climate.o \
	$(ENVIRONMENT_PATH)/config.o \
	$(ENVIRONMENT_PATH)/environment.o \
	$(ENVIRONMENT_PATH)/location.o \
	$(ENVIRONMENT_PATH)/plant_builder.o \
	$(ENVIRONMENT_PATH)/plant.o \
	$(ENVIRONMENT_PATH)/plant_container.o \
	$(ENVIRONMENT_PATH)/soil.o \
	$(ENVIRONMENT_PATH)/soil_container.o \
	$(ENVIRONMENT_PATH)/sun_info.o \
	$(ENVIRONMENT_PATH)/coordinate.o \
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
PHOTON_SIMULATOR_PATH := $(SIMULATOR_PATH)/photons
PHOTON_SIMULATOR_PHOTON_PATH := $(PHOTON_SIMULATOR_PATH)/photon
PHOTON_SIMULATOR_PHOTON_OBJ := $(PHOTON_SIMULATOR_PHOTON_PATH)/photon.o
THIRDPARTY_TINY_OBJ_LOADER_PATH := $(THIRD_PARTY_PATH)/tinyobjloader
THIRDPARTY_MATH_VECTOR_PATH := $(THIRD_PARTY_PATH)/Optimized-Photon-Mapping/src/math
THIRDPARTY_MATH_VECTOR_OBJ := $(PHOTON_SIMULATOR_PATH)/vector.o
THIRDPARTY_KDTREE_PATH := $(THIRD_PARTY_PATH)/KDTree
THIRDPARTY_KDTREE_OBJ := $(PHOTON_SIMULATOR_PATH)/KDTree.o
PHOTON_SIMULATOR_MODEL_PATH := $(PHOTON_SIMULATOR_PATH)/model
PHOTON_SIMULATOR_MODEL_OBJ := $(PHOTON_SIMULATOR_MODEL_PATH)/face.o\
	$(PHOTON_SIMULATOR_MODEL_PATH)/material.o\
	$(PHOTON_SIMULATOR_MODEL_PATH)/mesh.o\
	$(PHOTON_SIMULATOR_MODEL_PATH)/model.o\
	$(PHOTON_SIMULATOR_MODEL_PATH)/tiny_obj_loader.o
PHOTON_SIMULATOR_OBJ := $(THIRDPARTY_MATH_VECTOR_OBJ) \
	$(THIRDPARTY_KDTREE_OBJ) \
	$(PHOTON_SIMULATOR_MODEL_OBJ) \
	$(PHOTON_SIMULATOR_PHOTON_OBJ)
SIMULATOR_OBJ := $(SIMULATOR_PATH)/photon_simulator.o \
  $(PHOTON_SIMULATOR_OBJ)

ALL_OBJ :=	$(ENVIRONMENT_OBJ) \
	$(AGENT_OBJ) \
	$(ACTION_OBJ) \
	$(PLANTS_OBJ) \
	$(SIMULATOR_OBJ)

# unit tests
TEST_PATH := ./tests

TEST_AGENT_PATH := $(TEST_PATH)/agent
TEST_AGENT := $(TEST_AGENT_PATH)/agent_test \
	$(TEST_AGENT_PATH)/qlearning_test

TEST_AGENT_ACTIONS_PATH := $(TEST_AGENT_PATH)/actions
TEST_AGENT_ACTIONS := $(TEST_AGENT_ACTIONS_PATH)/crop_test

TEST_ENVIRONMENT_PATH := $(TEST_PATH)/environment
TEST_ENVIRONMENT := $(TEST_ENVIRONMENT_PATH)/climate_test \
	$(TEST_ENVIRONMENT_PATH)/config_test \
	$(TEST_ENVIRONMENT_PATH)/environment_test \
	$(TEST_ENVIRONMENT_PATH)/plant_container_test \
	$(TEST_ENVIRONMENT_PATH)/location_test \
	$(TEST_ENVIRONMENT_PATH)/soil_test \
	$(TEST_ENVIRONMENT_PATH)/sun_info_test \
	$(TEST_ENVIRONMENT_PATH)/terrain_test \
	$(TEST_ENVIRONMENT_PATH)/utility_test \
	$(TEST_ENVIRONMENT_PATH)/weather_test

TEST_ENVIRONMENT_PLANTS_PATH := $(TEST_ENVIRONMENT_PATH)/plants
TEST_ENVIRONMENT_PLANTS := $(TEST_ENVIRONMENT_PLANTS_PATH)/bean_test

TEST_SIMULATORS_PATH := $(TEST_PATH)/simulators
TEST_PHOTON_SIMULATOR_PATH := $(TEST_SIMULATORS_PATH)/photons
TEST_PHOTON_SIMULATOR_MODEL := $(TEST_PHOTON_SIMULATOR_PATH)/photon_simulator_model_test

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

$(PHOTON_SIMULATOR_MODEL_PATH)/tiny_obj_loader.o: $(THIRDPARTY_TINY_OBJ_LOADER_PATH)/tiny_obj_loader.cc $(THIRDPARTY_TINY_OBJ_LOADER_PATH)/tiny_obj_loader.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(PHOTON_SIMULATOR_PATH)/vector.o: $(THIRDPARTY_MATH_VECTOR_PATH)/vector.cpp $(THIRDPARTY_MATH_VECTOR_PATH)/vector.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(PHOTON_SIMULATOR_PATH)/KDTree.o: $(THIRDPARTY_KDTREE_PATH)/KDTree.cpp $(THIRDPARTY_KDTREE_PATH)/KDTree.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%_test: %_test.cc $(ALL_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TESTFLAGS) $(ALL_OBJ) $@.cc -o $@ $(TESTLD) $(OPENGLLIBS)

all: $(ALL_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(ALL_OBJ) $(MAIN_NAME).cc -o $(MAIN_NAME) $(OPENGLLIBS)

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
	rm -f $(ALL_OBJ) $(TEST_ALL)
	rm -f $(MAIN_NAME)
