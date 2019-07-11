import {
  Scene,
  PerspectiveCamera,
  WebGLRenderer,
  Object3D,
  Color,
  AmbientLight,
  PointLight,
  GridHelper,
  Camera,
  LineBasicMaterial
} from "three/src/Three";
import {OrbitControls} from 'three/examples/jsm/controls/OrbitControls';
import {TileMap} from './tilemap';
import {Configs} from './config';

export class Render {
  private camera: PerspectiveCamera;  // public for convenience
  private scene: Scene;
  private renderer: WebGLRenderer;
  private cameraControls: OrbitControls;

  configs: Configs;
  tilemap: TileMap | undefined;

  constructor(configs: Configs) {
    let container = document.createElement('div');
    document.body.appendChild(container);

    let canvasWidth = window.innerWidth;
    let canvasHeight = window.innerHeight;

    // CONFIGS
    this.configs = configs;

    // RENDERER
    this.renderer = new WebGLRenderer({antialias: true});
    this.renderer.setPixelRatio(window.devicePixelRatio);
    this.renderer.setSize(canvasWidth, canvasHeight);
    this.renderer.gammaInput = true;
    this.renderer.gammaOutput = true;
    container.appendChild(this.renderer.domElement);

    // CAMERA
    this.camera = new PerspectiveCamera(
        45, window.innerWidth / window.innerHeight, 1, 4000);
    this.camera.position.set(0, -100, 75);

    // CONTROLS
    this.cameraControls =
        new OrbitControls(this.camera, this.renderer.domElement);
    this.cameraControls.addEventListener('change', this.render.bind(this));

    // SCENE
    this.scene = new Scene();
    this.scene.background = new Color(0xAAAAAA);
    let ambientLight = new AmbientLight(0xcccccc, 0.4);
    this.scene.add(ambientLight);
    let pointLight = new PointLight(0xffffff, 0.8);
    this.camera.add(pointLight);
    this.scene.add(this.camera);

    // EVENTS
    window.addEventListener('resize', this.onWindowResize.bind(this), false);


    // GRID
    let grid = new GridHelper(100, 10, 0xFFFFFF, 0xFFFFFF);
    grid.geometry.rotateX(Math.PI / 2);
    if (grid.material instanceof LineBasicMaterial) {
      grid.material.linewidth = 10;
    } else {
      for (let material of<LineBasicMaterial[]>grid.material) {
        material.linewidth = 10;
      }
    }
    this.scene.add(grid);
  }

  getCamera(): Camera { return this.camera; }

  reset() {
    let plant: Object3D | undefined;
    for (let i = 0; i < this.configs.getHeight(); i++) {
      for (let j = 0; j < this.configs.getWidth(); j++) {
        plant = this.configs.removePlantModel(i, j);
        if (plant !== undefined) {
          this.scene.remove(plant);
        }
      }
    }
  }

  bindTileMapEvent(tilemap: TileMap) {
    this.tilemap = tilemap;
    document.addEventListener(
        'mousemove', this.tilemap.onDocumentMouseMove.bind(this.tilemap),
        false);
    document.addEventListener(
        'mousedown', this.tilemap.onDocumentMouseDown.bind(this.tilemap),
        false);
    document.addEventListener(
        'touchstart', this.tilemap.onDocumentTouchStart.bind(this.tilemap),
        false);
  }

  onWindowResize() {
    let canvasWidth = window.innerWidth;
    let canvasHeight = window.innerHeight;
    this.renderer.setSize(canvasWidth, canvasHeight);

    this.camera.aspect = canvasWidth / canvasHeight;
    this.camera.updateProjectionMatrix();

    this.render();
  }

  render() { this.renderer.render(this.scene, this.camera); }

  addtoScene(model: Object3D | undefined) {
    if (model !== undefined) {
      this.scene.add(model);
    }
  }

  removefromScene(model: Object3D | undefined) {
    if (model !== undefined) {
      this.scene.remove(model);
    }
  }
}
