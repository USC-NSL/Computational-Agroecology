This repo is for the computational agroecology project.

# How to build this project
## Setup steps

The following instructions are mainly for **Ubuntu 18.04 LTS**.

### Basic tools

```shell
$ [sudo] apt update
$ [sudo] apt install gcc g++ make
```

### gtest

**Fetch the source code**

```shell
$ [sudo] apt install libgtest-dev cmake
```

## Load submodule

```shell
$ git submodule update --init --recursive
```

**Installation**

```shell
$ cd /usr/src/gtest
$ [sudo] cmake CMakeLists.txt
$ [sudo] make
$ [sudo] cp *.a /usr/lib
```

### OpenGL

**Installation**

```shell
$ [sudo] apt-get install freeglut3 freeglut3-dev cmake libglew-dev mesa-common-dev build-essential libglew-dev libglm-dev
```

## Build
```shell
$ make
```
The binary executable file will be named `main` under the directory `environment`.

# Unit tests

## Build

```shell
$ make all_test
```

All test binary will be built under the ```tests``` directory.

## Run all tests
```shell
$ make all_test_run
```