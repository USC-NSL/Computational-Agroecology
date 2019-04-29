cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests dataReader.cpp)
add_executable(runTests1 plantClassifier.cpp)
add_executable(terrainTests terrain.cpp)
add_executable(configTests config_test.cc)
add_executable(plantTests plant.cpp)
add_executable(actionTests plant_adapter.cpp)
add_executable(weatherTests weatherTests.cpp)
add_executable(simulatorRunTests simulator_run.cpp)

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runTests1 ${GTEST_LIBRARIES} pthread)
target_link_libraries(terrainTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(configTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(plantTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(weatherTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(actionTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(simulatorRunTests ${GTEST_LIBRARIES} pthread)
