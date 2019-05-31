CXX := g++
CXXFLAGS := -std=c++11

INCLUDES := -I ./

OBJ := climate.o config.o environment.o location.o main.o plant.o soil.o terrain.o weather.o

PLANT_TYPE_PATH := ./plant_types
PLANT_TYPE_OBJ := $(PLANT_TYPE_PATH)/plant_type.o $(PLANT_TYPE_PATH)/bean.o $(PLANT_TYPE_PATH)/corn.o $(PLANT_TYPE_PATH)/squash.o

SIMULATOR_PATH := ./simulators
ACTION_PATH := ./simulators/actions
SIMULATOR_OBJ := $(SIMULATOR_PATH)/main_simulator.o $(ACTION_PATH)/action.o $(ACTION_PATH)/crop.o

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all: $(OBJ) $(PLANT_TYPE_OBJ) $(SIMULATOR_OBJ)
	$(CXX) $(OBJ) $(PLANT_TYPE_OBJ) $(SIMULATOR_OBJ) -o main
clean:
	rm -f $(OBJ) $(PLANT_TYPE_OBJ) $(SIMULATOR_OBJ)
	rm -f main