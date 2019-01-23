cmake_minimum_required(VERSION 3.12)
project(Computational_Agroecology)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(agent)
include_directories(sim)

add_executable(Computational_Agroecology
        agent/main.cc
        sim/plant.cc
        sim/plant.h
        action.cc
        action.h
        location.h)
