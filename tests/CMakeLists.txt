cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(dataReaderTests dataReader.cpp)
add_executable(plantClassifierTests plantClassifier.cpp)
add_executable(terrainTests terrain.cpp)
add_executable(configTests config_test.cc)
add_executable(plantTests plant.cpp)
add_executable(actionTests plant_adapter.cpp)
add_executable(weatherTests weatherTests.cpp)
add_executable(simulatorRunTests simulator_run.cpp)
add_executable(soilTests soilTests.cpp)
add_executable(plantIndexTests plantIndexTests.cpp)

target_link_libraries(dataReaderTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(plantClassifierTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(terrainTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(configTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(plantTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(weatherTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(actionTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(simulatorRunTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(soilTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(plantIndexTests ${GTEST_LIBRARIES} pthread)