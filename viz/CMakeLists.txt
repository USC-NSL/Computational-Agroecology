cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)
set(SOURCES photon.cpp
            photon_control.cpp
            map_control.cpp
            main.cpp)
# for including headers
include_directories(".")
# enable GDB
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(common)
add_subdirectory(model)
add_subdirectory(loader)

# Locate OpenGL GLUT GLEW libraries
find_package(OpenGL REQUIRED)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glut/include)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/glut/lib)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/glew/lib)
else ()
	find_package(GLUT REQUIRED)
	find_package(GLEW REQUIRED)
endif()
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# Link photon to libraries
add_executable(photon ${SOURCES})
target_link_libraries(photon common model loader)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	message("fix by yourself please.")
	target_link_libraries(photon freeglutd glew32)
else ()
	target_link_libraries(photon pthread)
	target_link_libraries(photon ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY})
endif()