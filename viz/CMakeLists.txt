cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)
set(SOURCES stdafx.h
	    Rendering_Control.cpp
            Rendering_Control.h 
            Rendering_Grids.cpp 
            Rendering_Grids.h 
            Rendering_Plants.cpp 
            Rendering_Plants.h 
            Rendering_Weather.h 
            Rendering_Weather.cpp 
	    stb_image.h
	    tiny_obj_loader.h
	    tiny_obj_loader.cc
            main.cpp)


# Locate OpenGL GLUT GLEW
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# Link vizTest with what we want to test and the GTest and pthread library
add_executable(vizTest ${SOURCES})

target_link_libraries(vizTest ${GTEST_LIBRARIES} pthread)
target_link_libraries(vizTest ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY})