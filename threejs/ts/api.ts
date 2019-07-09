import {Configs} from "./config";
import {
  CreateAgentRequest,
  CreateAgentResponse,
  CreateEnvironmentRequest,
  CreateEnvironmentResponse,
  GetEnvironmentRequest,
  GetEnvironmentResponse
} from '../js/agent_server_pb';
import {Environment} from '../js/environment_pb';
import {AgentServerClient} from '../js/agent_server_grpc_web_pb';
import {Error} from "grpc-web";

const status: {[index: number]: string} = {
  0: "OK",
  1: "CANCELLED",
  2: "UNKNOWN",
  3: "INVALID_ARGUMENT",
  4: "DEADLINE_EXCEEDED",
  5: "NOT_FOUND",
  6: "ALREADY_EXISTS",
  7: "PERMISSION_DENIED",
  16: "UNAUTHENTICATED",
  8: "RESOURCE_EXHAUSTED",
  9: "FAILED_PRECONDITION",
  10: "ABORTED",
  11: "OUT_OF_RANGE",
  12: "UNIMPLEMENTED",
  13: "INTERNAL",
  14: "UNAVAILABLE",
  15: "DATA_LOSS",
};

export class API {
  configs: Configs;
  server: AgentServerClient;
  agent_name: string;
  env_name: string;
  constructor(configs: Configs) {
    this.configs = configs;
    // Please change IP address and port, according to ./../envoy/envoy.yaml
    this.server =
        new AgentServerClient('http://192.168.136.128:8080', null, null);
    this.agent_name = 'Barath';
    this.env_name = "Happy Farm";
    this.createEnvironment();
    this.createAgent();
    this.getEnvironment();
  }

  createAgent() {
    let request = new CreateAgentRequest();
    request.setAgentName(this.agent_name);
    request.setEnvironmentName(this.env_name);
    this.server.createAgent(
        request, undefined, (err: Error, _response: CreateAgentResponse) => {
          if (err)
            console.log("createAgent ERROR " + status[err.code] + ": " +
                        err.message);
          else
            console.log("createAgent: agent " + this.agent_name + " created.");
        });
  }

  createEnvironment() {
    let request = new CreateEnvironmentRequest();
    request.setName(this.env_name);
    request.setTimestampEpochCount(Date.now());
    this.server.createEnvironment(
        request, undefined,
        (err: Error, _response: CreateEnvironmentResponse) => {
          if (err)
            console.log("createEnvironment ERROR " + status[err.code] + ": " +
                        err.message);
          else
            console.log("createEnvironment: environment " + this.env_name +
                        " created.");
        });
  }

  getEnvironment() {
    let request = new GetEnvironmentRequest();
    request.setName(this.env_name);
    this.server.getEnvironment(
        request, undefined, (err: Error, response: GetEnvironmentResponse) => {
          if (err)
            console.log("getEnvironment ERROR " + status[err.code] + ": " +
                        err.message);
          else {
            let environment = response.getEnvironment();
            if (environment !== undefined) this.updateScene(environment);
          }
        });
  }

  updateScene(environment: Environment) {
    let terrain = environment.getTerrain();
    if (terrain !== undefined) {
      terrain.getTilesList();
      console.log(terrain.toObject());
    }
  }

  print() {
    for (var i = 0; i < this.configs.getHeight(); i++)
      for (var j = 0; j < this.configs.getWidth(); j++)
        if (this.configs.getPlantType(i, j) != undefined)
          console.log(i, j, this.configs.getPlantType(i, j));
  }

  // send message to reset
  reset() { console.log("reset"); }

  send() { console.log("send"); }
  recv() { console.log("recieve"); }
}
