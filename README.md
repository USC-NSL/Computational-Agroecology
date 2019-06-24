This repo is for the computational agroecology project.

# Setup steps
The following instructions are mainly for **Ubuntu 18.04 LTS** with **gcc version 7.4.0**.

## Basic tools
```shell
$ [sudo] apt update
$ [sudo] apt install gcc g++ make
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

# Build
```shell
$ make all
```

**Currently, the only output will be a bunch of objects files and a dummy_main binary executable.**

# Run tests
## Build
```shell
$ make all_test
```

## Run
```shell
$ make all_test_run
```
