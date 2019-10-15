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

### gRPC (This is only required for agent server)

**Dependency**
```shell
$ [sudo] apt install build-essential autoconf libtool pkg-config
```
**Clone the gRPC repository (including submodules)**
```shell
$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc ~/grpc
$ cd ~/grpc
$ git submodule update --init
```
**Make and install gRPC**
```shell
$ make
$ [sudo] make install
```
**Protoc**
```shell
$ cd ~/grpc/third_party/protobuf
$ [sudo] make install
```

## Load submodule

```shell
$ git submodule update --init --recursive
```

## Build

### simulator
```shell
$ make
```
The binary executable file will be named `main` under the directory `environment`.

### agent grpc server
```shell
$ make agent_server
```
The binary executable will be named `agent_server` under the directory `agent_server`.

# Unit tests

## Build

### simulator
```shell
$ make all_test
```

### agent grpc server
```shell
$ make agent_server_test
```

All test binary will be built under the ```tests``` directory.

## Run all tests for simulator
```shell
$ make all_test_run
```

## Run all tests for agent grpc server
```shell
$ make agent_server_test_run
```
