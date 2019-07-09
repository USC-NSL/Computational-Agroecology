import {Object3D} from "Three";
import {Mode} from "./common";
import {Configs} from "./config";
import {Render} from "./render";
import {API} from "./api";
import {GUI} from "./GUI";
import {TileMap} from "./tilemap";

export class MainControl {
  // modules
  configs: Configs;
  api: API;
  gui: GUI;
  tilemap: TileMap;
  render: Render;

  constructor(width: number, height: number) {
    this.configs = new Configs(width, height);
    this.render = new Render(this.configs);
    this.api = new API(this.configs);
    this.gui = new GUI(this.render, this.api, this.reset.bind(this));
    this.tilemap =
        new TileMap(this.configs, this.render, this.updateTile.bind(this));
    // resolve cyclic dependency
    this.render.bindTileMapEvent(this.tilemap);
  }

  async load() {
    await this.configs.loadModels();
    await this.updateTile(0, 0, "CORN", 0);
    await this.updateTile(0, 1, "CORN", 3);
    await this.updateTile(0, 2, "CORN", 6);
    await this.updateTile(1, 0, "SQUASH", 0);
    await this.updateTile(1, 1, "SQUASH", 3);
    await this.updateTile(1, 2, "SQUASH", 6);
    this.render.render();
  }

  reset() {
    this.tilemap.reset();
    this.render.reset();
    this.configs.reset();  // reset after renderer
    this.api.reset();
  }

  updateTile(gridX: number, gridY: number,
             planttype: string = this.gui.getMode(), plantstatus: number = 0) {
    let plant: Object3D | undefined;
    switch (planttype) {
      case Mode[Mode.CORN]:
      case Mode[Mode.BEAN]:
      case Mode[Mode.SQUASH]:
        plant =
            this.configs.addPlantModel(planttype, plantstatus, gridX, gridY);
        if (plant !== undefined) {
          this.render.addtoScene(plant);
          this.api.agentAddCrop(gridX, gridY, planttype);
        }
        break;
      case Mode[Mode.WATER]:
        return this.configs.water(gridX, gridY);
      case Mode[Mode.REMOVE]:
        plant = this.configs.removePlantModel(gridX, gridY);
        if (plant !== undefined) {
          this.render.removefromScene(plant);
          this.api.agentRemoveCrop(gridX, gridY);
        }
        break;
      case Mode[Mode.HARVEST]:
        alert("method not implemented yet.");
        break;
      default:
        console.log("invalid mode: " + this.gui.getMode());
    }
    return undefined;
  }
}

let mainControl = new MainControl(10, 10);
mainControl.load();
