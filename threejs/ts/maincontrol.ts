import {Object3D} from "three/src/Three";
import {FunctionMode, WeatherMode} from "./common";
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
    this.gui = new GUI(this.render, this.api, this.reset.bind(this),
                       this.updateWeather.bind(this));
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
    this.render.animate();
  }

  reset() {
    this.tilemap.reset();
    this.render.reset();
    this.configs.reset();  // reset after renderer
    this.api.reset();
  }

  updateTile(gridX: number, gridY: number,
             planttype: string = this.gui.getFunctionMode(),
             plantstatus: number = 0) {
    let plant: Object3D | undefined;
    switch (planttype) {
      case FunctionMode[FunctionMode.CORN]:
      case FunctionMode[FunctionMode.BEAN]:
      case FunctionMode[FunctionMode.SQUASH]:
        plant =
            this.configs.addPlantModel(planttype, plantstatus, gridX, gridY);
        if (plant !== undefined) {
          this.render.addModeltoScene(plant);
          this.api.agentAddCrop(gridX, gridY, planttype);
        }
        break;
      case FunctionMode[FunctionMode.WATER]:
        return this.configs.water(gridX, gridY);
      case FunctionMode[FunctionMode.REMOVE]:
        plant = this.configs.removePlantModel(gridX, gridY);
        if (plant !== undefined) {
          this.render.removeModelfromScene(plant);
          this.api.agentRemoveCrop(gridX, gridY);
        }
        break;
      case FunctionMode[FunctionMode.HARVEST]:
        alert("method not implemented yet.");
        break;
      default:
        console.log("invalid mode: " + this.gui.getFunctionMode());
    }
    return undefined;
  }

  updateWeather(weatherMode: string) {
    switch (weatherMode) {
      case WeatherMode[WeatherMode.SUNNY]:
        this.render.addSuntoScene();
        this.render.removeCloudfromScene();
        this.render.removeRainfromScene();
        break;
      case WeatherMode[WeatherMode.CLOUDY]:
        this.render.addCloudtoScene();
        this.render.removeRainfromScene();
        this.render.removeSunfromScene();
        break;
      case WeatherMode[WeatherMode.RAINY]:
        this.render.addRaintoScene();
        this.render.addCloudtoScene();
        this.render.removeSunfromScene();
        break;
      default:
        break;
    }
  }
}

let mainControl = new MainControl(10, 10);
mainControl.load();
