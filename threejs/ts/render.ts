import {
  Scene,
  PerspectiveCamera,
  WebGLRenderer,
  Object3D,
  Color,
  PCFSoftShadowMap,
  AmbientLight,
  FogExp2,
} from "three/src/Three";
import {MapControls} from 'three/examples/jsm/controls/MapControls';

import {TileMap} from './tilemap';
import {PlantConfigs} from './plant';
import {color_configs} from "./common";
import {WeatherConfigs} from "./weather";


export class Render {
  camera: PerspectiveCamera;  // public for convenience
  scene: Scene;
  private renderer: WebGLRenderer;
  private mapControl: MapControls;

  plantConfigs: PlantConfigs;
  weatherConfigs: WeatherConfigs;
  tilemap: TileMap | undefined;

  constructor(plantConfigs: PlantConfigs, weatherConfigs: WeatherConfigs) {
    let container = document.createElement('div');
    container.setAttribute("id", "threejs");
    document.body.appendChild(container);

    // CONFIGS
    this.plantConfigs = plantConfigs;
    this.weatherConfigs = weatherConfigs;

    // SCENE
    this.scene = new Scene();
    this.scene.name = "scene";
    this.scene.background = new Color(color_configs.BACKGROUND);

    // RENDERER
    this.renderer = new WebGLRenderer({antialias: true});
    this.renderer.setPixelRatio(window.devicePixelRatio);
    this.renderer.setSize(window.innerWidth, window.innerHeight);
    this.renderer.shadowMap.enabled = true;
    this.renderer.shadowMap.type = PCFSoftShadowMap;
    this.renderer.gammaInput = true;
    this.renderer.gammaOutput = true;
    container.appendChild(this.renderer.domElement);

    // FOG
    this.scene.fog = new FogExp2(color_configs.BACKGROUND, 0.002);
    this.renderer.setClearColor(this.scene.fog.color);

    // CAMERA
    this.camera = new PerspectiveCamera(
        45, window.innerWidth / window.innerHeight, 1, 1000);
    this.camera.position.set(0, -160, 120);
    this.camera.up.set(0, 0, 1);

    // CAMERA CONTROLS
    this.mapControl = new MapControls(this.camera, this.renderer.domElement);
    this.mapControl.enableDamping = true;
    this.mapControl.dampingFactor = 0.25;
    this.mapControl.enableRotate = false;
    this.mapControl.minDistance = 60;
    this.mapControl.maxDistance = 200;

    // AMBIENT LIGHT
    let ambientlight = new AmbientLight(color_configs.AMBIENT_LIGHT, 0.5);
    this.scene.add(ambientlight);

    // EVENTS
    window.addEventListener('resize', this.onWindowResize.bind(this), false);

    this.animate();
  }

  reset() {
    let plant: Object3D | undefined;
    for (let i = 0; i < this.plantConfigs.getHeight(); i++) {
      for (let j = 0; j < this.plantConfigs.getWidth(); j++) {
        plant = this.plantConfigs.removePlantModel(i, j);
        if (plant !== undefined) {
          this.scene.remove(plant);
        }
      }
    }
  }

  bindTileMapEvent(tilemap: TileMap) {
    this.tilemap = tilemap;
    document.addEventListener(
        'mousedown', this.tilemap.onDocumentMouseDown.bind(this.tilemap),
        false);
    document.addEventListener(
        'mousemove', this.tilemap.onDocumentMouseMove.bind(this.tilemap),
        false);
    document.addEventListener(
        'mouseup', this.tilemap.onDocumentMouseUp.bind(this.tilemap), false);
    document.addEventListener(
        'touchstart', this.tilemap.onDocumentTouchStart.bind(this.tilemap),
        false);
    document.addEventListener(
        'touchmove', this.tilemap.onDocumentTouchMove.bind(this.tilemap),
        false);
    document.addEventListener(
        'touchend', this.tilemap.onDocumentTouchEnd.bind(this.tilemap), false);
  }

  onWindowResize() {
    let canvasWidth = window.innerWidth;
    let canvasHeight = window.innerHeight;
    this.renderer.setSize(canvasWidth, canvasHeight);

    this.camera.aspect = canvasWidth / canvasHeight;
    this.camera.updateProjectionMatrix();
  }

  animate() {
    requestAnimationFrame(this.animate.bind(this));
    this.weatherConfigs.renderWeather(this.camera);
    this.mapControl.update();
    this.renderer.render(this.scene, this.camera);
  }

  addModeltoScene(model: Object3D | undefined) {
    if (model !== undefined) {
      this.scene.add(model);
    }
  }

  removeModelfromScene(model: Object3D | undefined) {
    if (model !== undefined) {
      this.scene.remove(model);
    }
  }
}
