# Setup steps
The following instructions are mainly for **Ubuntu 18.04 LTS**.

## Basic tools
```shell
$ [sudo] apt update
$ [sudo] apt install gcc g++ make
```

## gRPC

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

# Build
```shell
$ make all
```

# Run tests
## Build
```shell
$ make all_test
```

All test binary will be built under the ```tests``` directory.