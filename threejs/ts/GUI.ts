import {Mode} from "./common";
import {Render} from "./render";
import {API} from "./api";
import * as dat from 'dat.gui';

export class GUI {
  mode: string;
  render: Render;
  api: API;
  cameraX: number;
  cameraY: number;
  cameraZ: number;
  restart: () => void;

  constructor(render: Render, api: API, restart: () => void) {
    this.mode = Mode[Mode.SQUASH];
    this.render = render;
    this.api = api;
    this.cameraX = 0;
    this.cameraY = 0;
    this.cameraZ = 0;
    this.restart = restart;

    var gui = new dat.GUI();

    gui.add(this, 'mode', Object.keys(Mode).filter(
                              (key: any) => { return isNaN(Number(key)); }));
    gui.add(this, 'update');
    gui.add(this, 'print');
    gui.add(this, 'restart');

    var table = gui.addFolder("Camera position");
    table.add(this, "cameraX", -1000.0, 1000.0, 1.0).name("cameraX").listen();
    table.add(this, "cameraY", -1000.0, 1000.0, 1.0).name("cameraY").listen();
    table.add(this, "cameraZ", -1000.0, 1000.0, 1.0).name("cameraZ").listen();

    this.updateCamera();
  }

  getMode(): string { return this.mode; }
  setMode(mode: Mode) { this.mode = Mode[mode]; }

  updateCamera() {
    requestAnimationFrame(this.updateCamera.bind(this));
    this.cameraX = this.render.getCamera().position.x;
    this.cameraY = this.render.getCamera().position.y;
    this.cameraZ = this.render.getCamera().position.z;
  };

  update() {
    console.log("Updating environment.");
    this.api.getEnvironment();
  };

  print(){this.api.print()};
}
