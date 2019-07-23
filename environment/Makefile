CXX := g++
CXXFLAGS := -std=c++17
OPENGLLIBS := -lGL -lglut -lGLEW

INCLUDES := -I ./

MAIN_OBJ := main.o
OBJ := climate.o config.o environment.o location.o plant.o soil.o terrain.o weather.o light.o

AGENT_PATH := ./agent
AGENT_OBJ := $(AGENT_PATH)/agent.o
QLEARNING_OBJ := $(AGENT_PATH)/q_learning.o

PLANT_TYPE_PATH := ./plant_types
PLANT_TYPE_OBJ := $(PLANT_TYPE_PATH)/plant_type.o $(PLANT_TYPE_PATH)/bean.o $(PLANT_TYPE_PATH)/corn.o $(PLANT_TYPE_PATH)/squash.o

SIMULATOR_PATH := ./simulators
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
ACTION_PATH := ./simulators/actions
SIMULATOR_OBJ := $(SIMULATOR_PATH)/main_simulator.o\
  $(SIMULATOR_PATH)/sun_simulator.o\
  $(ACTION_PATH)/action.o\
  $(ACTION_PATH)/crop.o\
  $(PHOTON_SIMULATOR_MODEL_OBJ)

TEST_PATH := ./tests
TEST_AGENT_PATH := $(TEST_PATH)/agent
TEST_AGENT_OBJ := $(TEST_AGENT_PATH)/agent_test.o
TEST_AGENT := $(TEST_AGENT_PATH)/agent_test
TEST_QLEARNING_OBJ := $(TEST_AGENT_PATH)/qlearning_test.o
TEST_QLEARNING := $(TEST_AGENT_PATH)/qlearning_test
TEST_PLANT_TYPES_PATH := $(TEST_PATH)/plant_types
TEST_PLANT_TYPES_OBJ := $(TEST_PLANT_TYPES_PATH)/bean_test.o $(TEST_PLANT_TYPES_PATH)/corn_test.o $(TEST_PLANT_TYPES_PATH)/squash_test.o $(TEST_PLANT_TYPES_PATH)/plant_type_test.o
TEST_PLANT_TYPES := $(TEST_PLANT_TYPES_PATH)/bean_test $(TEST_PLANT_TYPES_PATH)/corn_test $(TEST_PLANT_TYPES_PATH)/squash_test $(TEST_PLANT_TYPES_PATH)/plant_type_test
TEST_SIMULATORS_PATH := $(TEST_PATH)/simulators
TEST_SIMULATORS_OBJ := $(TEST_SIMULATORS_PATH)/main_simulator_test.o $(TEST_SIMULATORS_PATH)/sun_simulator_test.o
TEST_SIMULATORS := $(TEST_SIMULATORS_PATH)/main_simulator_test $(TEST_SIMULATORS_PATH)/sun_simulator_test
TEST_SIMULATORS_ACTIONS_PATH := $(TEST_SIMULATORS_PATH)/actions
TEST_SIMULATORS_ACTIONS_OBJ := $(TEST_SIMULATORS_ACTIONS_PATH)/crop_test.o
TEST_SIMULATORS_ACTIONS := $(TEST_SIMULATORS_ACTIONS_PATH)/crop_test
TEST_PHOTON_SIMULATOR_PATH := $(TEST_SIMULATORS_PATH)/photon_simulator
TEST_PHOTON_SIMULATOR_MODEL_OBJ := $(TEST_PHOTON_SIMULATOR_PATH)/photon_simulator_model_test.o
TEST_PHOTON_SIMULATOR_MODEL := $(TEST_PHOTON_SIMULATOR_PATH)/photon_simulator_model_test
TEST_OBJ := $(TEST_PATH)/climate_test.o $(TEST_PATH)/config_test.o $(TEST_PATH)/environment_test.o $(TEST_PATH)/location_test.o $(TEST_PATH)/plant_test.o $(TEST_PATH)/soil_test.o $(TEST_PATH)/terrain_test.o $(TEST_PATH)/utility_test.o $(TEST_PATH)/light_test.o $(TEST_PATH)/weather_test.o $(TEST_SIMULATORS_OBJ) $(TEST_SIMULATORS_ACTIONS_OBJ) $(TEST_PLANT_TYPES_OBJ) $(TEST_AGENT_OBJ) $(TEST_PHOTON_SIMULATOR_MODEL_OBJ) $(TEST_QLEARNING_OBJ)
TEST_ALL := $(TEST_PATH)/climate_test $(TEST_PATH)/config_test $(TEST_PATH)/environment_test $(TEST_PATH)/location_test $(TEST_PATH)/plant_test $(TEST_PATH)/soil_test $(TEST_PATH)/terrain_test $(TEST_PATH)/utility_test $(TEST_PATH)/light_test $(TEST_PATH)/weather_test $(TEST_SIMULATORS) $(TEST_SIMULATORS_ACTIONS) $(TEST_PLANT_TYPES) $(TEST_AGENT) $(TEST_PHOTON_SIMULATOR_MODEL) $(TEST_QLEARNING)
TESTFLAGS := -Igtest/include
TESTLD := -lgtest -lpthread

ALL_OBJ_WITHOUT_MAIN := $(OBJ) $(AGENT_OBJ) $(PLANT_TYPE_OBJ) $(SIMULATOR_OBJ) $(QLEARNING_OBJ)
ALL_OBJ := main.o $(ALL_OBJ_WITHOUT_MAIN)

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

all_test: $(TEST_ALL)

all_test_run: $(TEST_ALL)
	@for var in $(TEST_ALL); do \
		echo $$var; \
		$$var; \
	done

clean:
	rm -f $(ALL_OBJ) $(TEST_OBJ) $(TEST_ALL)
	rm -f main
