// TODO: move client.js to /public
var PROTO_PATH = __dirname + './../public/proto/agent_server.proto';

var grpc = require('grpc');
var protoLoader = require('@grpc/proto-loader');
var packageDefinition = protoLoader.loadSync(
    PROTO_PATH,
    {
        keepCase: true,
        longs: String,
        enums: String,
        defaults: true,
        oneofs: true
    });
var grpc_proto = grpc.loadPackageDefinition(packageDefinition).agent_server.service;

function main() {
    var client = new grpc_proto.AgentServer('0.0.0.0:50000',
        grpc.credentials.createInsecure());
    var agent;
    if (process.argv.length >= 3) {
        agent = process.argv[2];
    } else {
        agent = 'Barath';
    }
    client.CreateAgent({ agent_name: agent, environment_name: agent },
        function (_err: any, _response: {}) {
        });
    client.CreateAgent({ agent_name: agent, environment_name: agent },
        function (_err: any, _response: {}) {
        });
}

main();
