import {FunctionMode, WeatherMode} from "./common";
import {Render} from "./render";
import {API} from "./api";
import * as dat from 'dat.gui';

export class GUI {
  functionMode: string;
  weatherMode: string;
  render: Render;
  api: API;
  cameraX: number;
  cameraY: number;
  cameraZ: number;
  restart: () => void;

  constructor(render: Render, api: API, restart: () => void) {
    this.functionMode = FunctionMode[FunctionMode.SQUASH];
    this.weatherMode = WeatherMode[WeatherMode.SUNNY];
    this.render = render;
    this.api = api;
    this.cameraX = 0;
    this.cameraY = 0;
    this.cameraZ = 0;
    this.restart = restart;

    let gui = new dat.GUI();

    gui.add(this, 'functionMode',
            Object.keys(FunctionMode)
                .filter((key: any) => { return isNaN(Number(key)); }));
    gui.add(this, 'weatherMode',
            Object.keys(WeatherMode)
                .filter((key: any) => { return isNaN(Number(key)); }))
        .onChange(this.updateWeather.bind(this));
    // reactivate updateWeather
    this.weatherMode = WeatherMode[WeatherMode.SUNNY];
    gui.add(this, 'update');
    gui.add(this, 'print');
    gui.add(this, 'restart');

    var table = gui.addFolder("Camera position");
    table.add(this, "cameraX", -1000.0, 1000.0, 1.0).name("cameraX").listen();
    table.add(this, "cameraY", -1000.0, 1000.0, 1.0).name("cameraY").listen();
    table.add(this, "cameraZ", -1000.0, 1000.0, 1.0).name("cameraZ").listen();

    this.updateCamera();
  }

  getFunctionMode(): string { return this.functionMode; }
  setFunctionMode(functionMode: FunctionMode) {
    this.functionMode = FunctionMode[functionMode];
  }

  updateCamera() {
    requestAnimationFrame(this.updateCamera.bind(this));
    this.cameraX = this.render.getCamera().position.x;
    this.cameraY = this.render.getCamera().position.y;
    this.cameraZ = this.render.getCamera().position.z;
  };

  updateWeather() {
    console.log(this.weatherMode);
    switch (this.weatherMode) {
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

  update() {
    console.log("Updating environment.");
    this.api.getEnvironment();
  };

  print(){this.api.print()};
}
