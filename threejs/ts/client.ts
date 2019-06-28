const {CreateAgentRequest, CreateAgentResponse} = require('./agent_server_pb.js');;
const {AgentServerClient} = require('./agent_server_grpc_web_pb.js');

var createAgentService = new AgentServerClient('http://localhost:50000');

var request = new CreateAgentRequest();
request.setAgentName('Barath');
request.setEnvironmentName('Happy Farm');

createAgentService.createAgent(request, {}, function(_err: any, _response: any) {
  // ...
});
