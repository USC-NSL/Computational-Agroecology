import {Object3D} from "three/src/Three";
import {FunctionMode, WeatherMode} from "./common";
import {PlantConfigs} from "./plant";
import {Render} from "./render";
import {API} from "./API";
import {GUI} from "./GUI";
import {TileMap} from "./tilemap";
import {WeatherConfigs} from "./weather";
import {SideNavigator} from "./side_navigator";
import {Initerary} from "./initerary";

export class MainControl {
  plantConfigs: PlantConfigs;
  weatherConfigs: WeatherConfigs;
  api: API;
  gui: GUI;
  tilemap: TileMap;
  render: Render;
  sideNavigator: SideNavigator;
  initerary: Initerary;

  constructor(width: number, height: number) {
    this.plantConfigs = new PlantConfigs(width, height);
    this.weatherConfigs = new WeatherConfigs(width, height);
    this.render = new Render(this.plantConfigs, this.weatherConfigs);
    this.sideNavigator = new SideNavigator(this.plantConfigs);
    this.api = new API(this.plantConfigs);
    this.gui = new GUI(this.render, this.api, this.reset.bind(this),
                       this.updateWeather.bind(this));
    this.tilemap = new TileMap(this.plantConfigs, this.render,
                               this.sideNavigator, this.updatePlant.bind(this));
    this.initerary = new Initerary();

    // resolve cyclic dependency
    this.render.bindTileMapEvent(this.tilemap);
    this.weatherConfigs.bindingCamera(this.render.camera);
  }

  async load() {
    await this.plantConfigs.loadModels();
    await this.updatePlant(0, 0, "CORN", 0);
    await this.updatePlant(0, 1, "CORN", 3);
    await this.updatePlant(0, 2, "CORN", 6);
    await this.updatePlant(1, 0, "SQUASH", 0);
    await this.updatePlant(1, 1, "SQUASH", 3);
    await this.updatePlant(1, 2, "SQUASH", 6);
    this.render.animate();
  }

  reset() {
    this.tilemap.reset();
    this.render.reset();
    this.plantConfigs.reset();  // reset after renderer
    this.api.reset();
  }

  updatePlant(gridX: number, gridY: number,
              planttype: string = this.gui.getFunctionMode(),
              plantstatus: number = 0) {
    let plant: Object3D | undefined;
    switch (planttype) {
      case FunctionMode[FunctionMode.CORN]:
      case FunctionMode[FunctionMode.BEAN]:
      case FunctionMode[FunctionMode.SQUASH]:
        plant = this.plantConfigs.addPlantModel(planttype, plantstatus, gridX,
                                                gridY);
        if (plant !== undefined) {
          this.render.addModeltoScene(plant);
          this.api.agentAddCrop(gridX, gridY, planttype);
        }
        break;
      case FunctionMode[FunctionMode.WATER]:
        return this.plantConfigs.water(gridX, gridY);
      case FunctionMode[FunctionMode.REMOVE]:
        plant = this.plantConfigs.removePlantModel(gridX, gridY);
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
        this.weatherConfigs.addSuntoScene(this.render.scene);
        this.weatherConfigs.removeCloudfromScene(this.render.scene);
        this.weatherConfigs.removeRainfromScene(this.render.scene);
        break;
      case WeatherMode[WeatherMode.CLOUDY]:
        this.weatherConfigs.addCloudtoScene(this.render.scene);
        this.weatherConfigs.removeRainfromScene(this.render.scene);
        this.weatherConfigs.removeSunfromScene(this.render.scene);
        break;
      case WeatherMode[WeatherMode.RAINY]:
        this.weatherConfigs.addRaintoScene(this.render.scene);
        this.weatherConfigs.addCloudtoScene(this.render.scene);
        this.weatherConfigs.removeSunfromScene(this.render.scene);
        break;
      default:
        break;
    }
  }
}

let mainControl = new MainControl(4, 16);
mainControl.load();
