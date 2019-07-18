import {FunctionMode, WeatherMode} from "./common";
import {Render} from "./render";
import {API} from "./API";
import * as dat from 'dat.gui';

export class GUI {
  functionMode: string;
  weatherMode: string;
  render: Render;
  api: API;
  cameraX: number;
  cameraY: number;
  cameraZ: number;
  restartCall: () => void;
  updateWeatherCall: (weatherMode: string) => void;

  constructor(render: Render, api: API, restartCall: () => void,
              updateWeatherCall: (weatherMode: string) => void) {
    this.functionMode = FunctionMode[FunctionMode.SQUASH];
    this.weatherMode = WeatherMode[WeatherMode.SUNNY];
    this.render = render;
    this.api = api;
    this.cameraX = 0;
    this.cameraY = 0;
    this.cameraZ = 0;
    this.restartCall = restartCall;
    this.updateWeatherCall = updateWeatherCall;

    let gui = new dat.GUI();

    gui.add(this, 'functionMode',
            Object.keys(FunctionMode)
                .filter((key: any) => { return isNaN(Number(key)); }));
    gui.add(this, 'weatherMode',
            Object.keys(WeatherMode)
                .filter((key: any) => { return isNaN(Number(key)); }))
        .onChange(this.updateWeather.bind(this));
    gui.add(this, 'update');
    gui.add(this, 'print');
    gui.add(this, 'restart');

    var table = gui.addFolder("Camera position");
    table.add(this, "cameraX", -1000.0, 1000.0, 1.0).name("cameraX").listen();
    table.add(this, "cameraY", -1000.0, 1000.0, 1.0).name("cameraY").listen();
    table.add(this, "cameraZ", -1000.0, 1000.0, 1.0).name("cameraZ").listen();

    this.updateCamera();
    this.updateWeather();
  }

  getFunctionMode(): string { return this.functionMode; }
  setFunctionMode(functionMode: FunctionMode) {
    this.functionMode = FunctionMode[functionMode];
  }

  // TODO: deprecate it later
  updateCamera() {
    requestAnimationFrame(this.updateCamera.bind(this));
    this.cameraX = this.render.camera.position.x;
    this.cameraY = this.render.camera.position.y;
    this.cameraZ = this.render.camera.position.z;
  };

  update() {
    console.log("Updating environment.");
    this.api.getEnvironment();
  };

  print(){this.api.print()};

  restart(){this.restartCall()};

  updateWeather() { this.updateWeatherCall(this.weatherMode); }
}
