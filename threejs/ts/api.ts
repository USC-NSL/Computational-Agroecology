import {Configs} from "./config";

export class API {
  configs: Configs;
  constructor(configs: Configs) { this.configs = configs; }

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
