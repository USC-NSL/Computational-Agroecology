# Setup steps
The following instructions are mainly for **Ubuntu 18.04 LTS**.

## Basic tools
```shell
$ [sudo] apt update
$ [sudo] apt install gcc g++ make
**If you want to use CMake tools**
$ [sudo] apt-get install cmake
```
## gtest
**Fetch the source code**
```shell
$ [sudo] apt install libgtest-dev cmake
```
**Installation**
```shell
$ cd /usr/src/gtest
$ [sudo] cmake CMakeLists.txt
$ [sudo] make
$ [sudo] cp *.a /usr/lib
```
## Add tests using CMake tools
**Add the lines of code in the CMakeLists.txt in the directory environment/**
```shell
$ add_executable(YOUR_EXECUTABLE_FILE ${DIR_ENVIRONMENT_SRCS})
$ target_link_libraries(YOUR_EXECUTABLE_FILE plant_types simulators YOUR_LIB_NAME)
```
**Also, Add the lines of code in the CMakeLists.txt in the subdirectory /tests/**
```shell
$ add_library(tests_simulators_sun_simulator ./simulators/sun_simulator_test.cc)
```
***If the platform is Windows***
```shell
$ target_link_libraries(YOUR_LIB_NAME gtestd gtest_maind)
```
***otherwise***
```shell
$ target_link_libraries(YOUR_LIB_NAME ${GTEST_LIBRARY})
```
# Run tests
## Build
```shell
$ make all_test
**If you want to use Cmake tools**
$ cmake CMakeLists.txt
```

All test binary will be built under the ```tests``` directory.
**If you use CMake tools, it will be right at all subdirectory**
