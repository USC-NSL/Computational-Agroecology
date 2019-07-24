import {PlantConfigs} from "./plant";
import {
  CreateAgentRequest,
  CreateEnvironmentRequest,
  GetEnvironmentRequest,
  GetEnvironmentResponse,
  SimulateToTimeRequest,
  AgentActionConfig,
  AgentAddCropRequest,
  AgentRemoveCropRequest,
  DeleteEnvironmentRequest,
  DeleteAgentRequest
} from '../js/agent_server_pb';
import {Environment, Terrain} from '../js/environment_pb';
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
  configs: PlantConfigs;
  server: AgentServerClient;
  agent_name = 'Barath';
  env_name = "Happy Farm";
  constructor(configs: PlantConfigs) {
    this.configs = configs;
    /////////////  change the IP address to your server's IP
    this.server = new AgentServerClient('http://204.57.3.150:8080', null, null);
    this.init();
  }

  async init() {
    await this.deleteEnvironment();
    await this.deleteAgent();
    await this.createEnvironment();
    await this.createAgent();
  }

  createAgent() {
    let request = new CreateAgentRequest();
    request.setAgentName(this.agent_name);
    request.setEnvironmentName(this.env_name);
    this.server.createAgent(request, undefined, (err: Error) => {
      if (err) {
        console.log("createAgent ERROR " + status[err.code] + ": " +
                    err.message);
      } else {
        console.log("createAgent: " + this.agent_name);
      }
    });
  }

  deleteAgent() {
    let request = new DeleteAgentRequest();
    request.setName(this.agent_name);
    this.server.deleteAgent(request, undefined, (err: Error) => {
      if (err) {
        console.log("deleteAgent ERROR " + status[err.code] + ": " +
                    err.message);
      } else {
        console.log("deleteAgent: " + this.agent_name);
      }
    });
  }

  createEnvironment() {
    let request = new CreateEnvironmentRequest();
    request.setName(this.env_name);
    request.setTimestampEpochCount(Date.now());
    let terrain = new Terrain();
    for (let i = 0; i < this.configs.getHeight(); i++) {
      let row = new Terrain.Row();
      for (let j = 0; j < this.configs.getWidth(); j++) {
        let cell = new Terrain.Cell();
        row.addCells(cell);
      }
      terrain.addTiles(row);
    }
    request.setTerrain(terrain);
    this.server.createEnvironment(request, undefined, (err: Error) => {
      if (err) {
        console.log("createEnvironment ERROR " + status[err.code] + ": " +
                    err.message);
      } else {
        console.log("createEnvironment: " + this.env_name);
      }
    });
  }

  getEnvironment() {
    let request = new GetEnvironmentRequest();
    request.setName(this.env_name);
    this.server.getEnvironment(
        request, undefined, (err: Error, response: GetEnvironmentResponse) => {
          if (err) {
            console.log("getEnvironment ERROR " + status[err.code] + ": " +
                        err.message);
          } else {
            let environment = response.getEnvironment();
            if (environment !== undefined) {
              this.updateScene(environment);
            }
          }
        });
  }

  deleteEnvironment() {
    let request = new DeleteEnvironmentRequest();
    request.setName(this.env_name);
    this.server.deleteEnvironment(request, undefined, (err: Error) => {
      if (err) {
        console.log("deleteEnvironment ERROR " + status[err.code] + ": " +
                    err.message);
      } else {
        console.log("deleteEnvironment: " + this.env_name);
      }
    });
  }

  updateScene(environment: Environment) {
    let terrain = environment.getTerrain();
    if (terrain !== undefined) {
      let rows = terrain.getTilesList();
      let length = rows.length;
      let width = rows[0].getCellsList().length;
      console.log("env: length " + length + ", width " + width);
    }
  }

  simulateToTime(timestamp: number) {
    let request = new SimulateToTimeRequest();
    request.setEnvironmentName(this.env_name);
    request.setTimestampEpochCount(timestamp);
    this.server.simulateToTime(request, undefined, (err: Error) => {
      if (err) {
        console.log("simulateToTime ERROR " + status[err.code] + ": " +
                    err.message);
      } else {
        console.log("simulateToTime: " + timestamp);
      }
    });
  }

  agentAddCrop(gridX: number, gridY: number, planttype: string) {
    let request = new AgentAddCropRequest();
    request.setAgentName(this.agent_name);
    request.setCropTypeName(planttype);
    let config = new AgentActionConfig();
    let coordinate = new Terrain.Coordinate();
    coordinate.setX(gridX);
    coordinate.setY(gridY);
    config.addAppliedRange(coordinate);
    config.setStartTimeEpochCount(Date.now());
    config.setEndTimeEpochCount(Date.now());
    request.setActionConfig(config);
    this.server.agentAddCrop(request, undefined, (err: Error) => {
      if (err) {
        console.log("agentAddCrop ERROR " + status[err.code] + ": " +
                    err.message);
      } else {
        console.log("agentAddCrop: " + planttype + " at (" + gridX + "," +
                    gridY + ")");
      }
    });
  }

  agentRemoveCrop(gridX: number, gridY: number) {
    let request = new AgentRemoveCropRequest();
    request.setAgentName(this.agent_name);
    let config = new AgentActionConfig();
    let coordinate = new Terrain.Coordinate();
    coordinate.setX(gridX);
    coordinate.setY(gridY);
    config.addAppliedRange(coordinate);
    config.setStartTimeEpochCount(Date.now());
    config.setEndTimeEpochCount(Date.now());
    request.setActionConfig(config);
    this.server.agentRemoveCrop(request, undefined, (err: Error) => {
      if (err) {
        console.log("agentRemoveCrop ERROR " + status[err.code] + ": " +
                    err.message);
      } else {
        console.log("agentRemoveCrop: (" + gridX + "," + gridY + ")");
      }
    });
  }

  print() {
    for (var i = 0; i < this.configs.getHeight(); i++) {
      for (var j = 0; j < this.configs.getWidth(); j++) {
        if (this.configs.getPlantType(i, j) != undefined) {
          console.log(i, j, this.configs.getPlantType(i, j));
        }
      }
    }
  }

  // send message to reset
  reset() { console.log("reset"); }

  send() { console.log("send"); }
}
