import {CreateAgentRequest, CreateAgentResponse} from '../js/agent_server_pb.js';
import {AgentServerClient} from '../js/agent_server_grpc_web_pb.js';
import {Status, Error} from "grpc-web";

// Please change IP address and port, according to ./../envoy/envoy.yaml
var createAgentService =
    new AgentServerClient('http://192.168.136.128:8080', null, null);

var request = new CreateAgentRequest();
request.setAgentName('Barath');
request.setEnvironmentName('Happy Farm');

var call = createAgentService.createAgent(
    request, undefined, function(err: Error, response: CreateAgentResponse) {
      if (err) {
        console.log(err.code);
        console.log(err.message);
      } else
        console.log(response.toObject());
    });
call.on("status", function(status: Status) {
  console.log("status.code: " + status.code);
  console.log("status.details: " + status.details);
  console.log("status.metadata: " + status.metadata);
});
