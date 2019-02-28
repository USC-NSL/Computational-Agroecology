cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests dataReader.cpp)
add_executable(runTests1 plantClassifier.cpp)

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runTests1 ${GTEST_LIBRARIES} pthread)