const {CreateAgentRequest, CreateAgentResponse} = require('./agent_server_pb.js');;
const {AgentServerClient} = require('./agent_server_grpc_web_pb.js');

var createAgentService = new AgentServerClient('http://localhost:8080');

var request = new CreateAgentRequest();
request.setMessage('Barath', "Barath");

createAgentService.echo(request, {}, function(_err: any, _response: any) {
  // ...
});
