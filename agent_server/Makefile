CXX := g++
CXXFLAGS := -std=c++17
LDFLAGS := `pkg-config --libs protobuf grpc++`

PROTOC := protoc
PROTO_PATH := ./proto
PROTO_FLAGS := --proto_path=$(PROTO_PATH) --cpp_out=$(PROTO_PATH)
GRPC_FLAGS := --proto_path=$(PROTO_PATH) --grpc_out=$(PROTO_PATH) --plugin=protoc-gen-grpc=`which grpc_cpp_plugin`

TESTFLAGS := -Igtest/include
TESTLD := -lgtest -lpthread

INCLUDES := -I ../ -I ../environment -I ./

%.pb.cc: %.proto
	$(PROTOC) $(PROTO_FLAGS) $<

%.grpc.pb.cc: %.proto %.pb.cc
	$(PROTOC) $(GRPC_FLAGS) $<

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%_test.o: %_test.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TESTFLAGS) -c $< -o $@

%_test: %_test.o agent_server_service
	$(CXX) $< agent_server.o message_convertor.o ../environment/environment.o ../environment/agent/agent.o ../environment/simulators/main_simulator.o ../environment/simulators/sun_simulator.o ../environment/plant_types/plant_type.o ../environment/location.o ../environment/plant.o ../environment/soil.o ../environment/terrain.o ../environment/climate.o ../environment/weather.o ../environment/config.o ../environment/simulators/actions/action.o proto/agent_server.grpc.pb.o proto/agent_server.pb.o proto/environment.pb.o -o $@ $(TESTLD) $(LDFLAGS)

agent_server_service: $(PROTO_PATH)/environment.pb.o $(PROTO_PATH)/agent_server.pb.o $(PROTO_PATH)/agent_server.grpc.pb.o agent_server.o agent_server_grpc_service.o message_convertor.o

all: agent_server_service
# tempprarily hard code them
	$(CXX) agent_server.o agent_server_grpc_service.o message_convertor.o ../environment/environment.o ../environment/agent/agent.o ../environment/simulators/main_simulator.o ../environment/plant_types/plant_type.o ../environment/location.o ../environment/plant.o ../environment/soil.o ../environment/terrain.o ../environment/climate.o ../environment/weather.o ../environment/config.o ../environment/simulators/actions/action.o proto/agent_server.grpc.pb.o proto/agent_server.pb.o proto/environment.pb.o $(LDFLAGS) -o agent_server

all_test: tests/agent_server_test tests/message_convertor_test

clean:
	rm -f *.o agent_server tests/agent_server_test tests/message_convertor_test
	rm -f $(PROTO_PATH)/*.h $(PROTO_PATH)/*.cc $(PROTO_PATH)/*.o
