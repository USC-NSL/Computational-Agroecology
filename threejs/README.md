### Dependencies

#### Node.js

For Linux user, you might need following dependcies

```bash
sudo apt install npm
sudo apt install node-typescript
```

For Windows user, you only need to install Node.js by yourself.

#### GRPC

Please follow the instruction in `/agent_server/README.md`.



### Build distributed code

run following commands in bash:

```bash
npm install
npm run build
npm run protoc
npm run bundle
```

copy the whole `/public` subdirectory to deployment path

### Node.js based compilation and test 

After compilation, run following commands in bash:

```bash
npm run start
```

open browser, enter `localhost:8000`

### GRPC client test

```bash
npm run client
```