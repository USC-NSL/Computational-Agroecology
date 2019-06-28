## Development Dependencies

#### Node.js

For **Linux** user,

```bash
sudo apt install npm
sudo apt install node-typescript
npm install -g npx
```

For **Windows** user, Please install [Node.js](<https://nodejs.org/en/download/>), and install `npx` by 

```powershell
npm install -g npx
```

#### gRPC

For **Linux** user, please follow the instruction in `../agent_server/README.md`.

For **Windows** user, please download [protoc.exe](https://github.com/protocolbuffers/protobuf/releases), and make sure it is  executable and discoverable in current path. 

#### gRPC-web

For **Linux** User,

```bash
git clone https://github.com/grpc/grpc-web.git
cd grpc-web
sudo make install-plugin
```

For **Windows** User, please download [protoc-gen-grpc-web.exe](<https://github.com/grpc/grpc-web/releases>) plugin, and make sure it is  executable and discoverable in current path. 

#### Version

For **Windows**, the workflow  is ensured with following version

+ npm  6.4.1
+ [Protocol Buffers v3.8.0](https://github.com/protocolbuffers/protobuf/releases/tag/v3.8.0)
+ [grpc/grpc-web 1.0.4](https://github.com/grpc/grpc-web/releases/tag/1.0.4)

For **Linux**,



## Build distributed code

### Compilation

run following commands in shell:

```bash
npm install
npm run build

# for windows user instead
npm run build-windows
```

copy the whole `/public` subdirectory to deployment path

### Node.js based compilation and test 

After compilation, run following commands in shell:

```shell
npm run start
```

open browser, enter `localhost:8000`

### gRPC client test

```bash
npm run client
```



## Deployment Dependencies

**Assume that the site is deployed on `Ubuntu 18.04`.**


