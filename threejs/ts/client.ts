const {CreateAgentRequest, CreateAgentResponse} =
    require('./agent_server_pb.js');
;
const {AgentServerClient} = require('./agent_server_grpc_web_pb.js');

// Please change IP address and port, according to ./../envoy/envoy.yaml
var createAgentService = new AgentServerClient('http://192.168.136.128:8080');

var request = new CreateAgentRequest();
request.setAgentName('Barath');
request.setEnvironmentName('Happy Farm');

var call = createAgentService.createAgent(
    request, {}, function(err:
                          {
                            code: any;
                            message: any;
                          },
                          response: { getMessage: () => void; }) {
      if (err) {
        console.log(err.code);
        console.log(err.message);
      } else {
        console.log(response.getMessage());
      }
    });
call.on('status', function(status:
                           {
                             code: any;
                             details: any;
                             metadata: any;
                           }) {
  console.log("status.code: " + status.code);
  console.log("status.details: " + status.details);
  console.log("status.metadata: " + status.metadata);
});
