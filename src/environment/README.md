# Setup steps
The following instructions are mainly for **Ubuntu 18.04 LTS**.

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

# Run tests
## Build
```shell
$ make all_test
```

All test binary will be built under the ```tests``` directory.
