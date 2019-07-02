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

- npm  6.4.1
- [Protocol Buffers v3.8.0](https://github.com/protocolbuffers/protobuf/releases/tag/v3.8.0)
- [grpc/grpc-web 1.0.4](https://github.com/grpc/grpc-web/releases/tag/1.0.4)

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

#### Nginx

Install

```bash
sudo apt update
sudo apt install nginx
```

Update firewall config

```bash
sudo ufw allow 'Nginx HTTP'
sudo ufw status
```

#### Docker

Reference: [How To Install and Use Docker on Ubuntu 18.04](<https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04>)

```bash
sudo apt update
sudo apt install apt-transport-https ca-certificates curl software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu bionic stable"
sudo apt update
sudo apt install docker-ce

# check status
sudo systemctl status docker

# execute Docker without sudo (optional)
sudo usermod -aG docker ${USER} # $USER for current username
```

#### Envoy

Install image

```bash
[sudo] docker pull envoyproxy/envoy
```

build new image and run

```bash
 cd envoy
 # modify address to static IP of your server in ./envoy.Dockerfile
 docker build -t agent/envoy -f ./envoy.Dockerfile .
 docker run -d -p 8080:8080 --network=host agent/envoy
 
 # for windows or mac
 docker run -d -p 8080:8080 agent/envoy
```

