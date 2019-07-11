import {
  Scene,
  PerspectiveCamera,
  WebGLRenderer,
  Object3D,
  Color,
  GridHelper,
  Camera,
  LineBasicMaterial,
  DirectionalLight,
  PCFSoftShadowMap,
  DirectionalLightShadow,
  AmbientLight,
  SphereBufferGeometry,
  MeshBasicMaterial,
  Mesh,
} from "three/src/Three";
import {OrbitControls} from 'three/examples/jsm/controls/OrbitControls';
import {TileMap} from './tilemap';
import {Configs} from './config';

export class Render {
  private camera: PerspectiveCamera;  // public for convenience
  private scene: Scene;
  private renderer: WebGLRenderer;
  private cameraControls: OrbitControls;
  private sun: DirectionalLight;

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
    this.renderer.shadowMap.enabled = true;
    this.renderer.shadowMap.type = PCFSoftShadowMap;
    this.renderer.gammaInput = true;
    this.renderer.gammaOutput = true;
    container.appendChild(this.renderer.domElement);

    // CAMERA
    this.camera = new PerspectiveCamera(
        45, window.innerWidth / window.innerHeight, 1, 4000);
    this.camera.position.set(0, -100, 75);

    // CAMERA CONTROLS
    this.cameraControls =
        new OrbitControls(this.camera, this.renderer.domElement);
    this.cameraControls.update();

    // SCENE
    this.scene = new Scene();
    this.scene.background = new Color(0xAAAAAA);

    // AMBIENT LIGHT
    let ambientlight = new AmbientLight(0xffffff, 0.1);
    this.scene.add(ambientlight);

    // SUN
    this.sun = new DirectionalLight(0xffd69c, 1.9);
    this.sun.castShadow = true;
    this.sun.position.set(0, 0, 0);
    this.sun.matrixAutoUpdate = true;
    this.sun.shadow = new DirectionalLightShadow(this.camera);
    this.sun.shadow.mapSize.width = 1024;
    this.sun.shadow.mapSize.height = 1024;
    this.sun.shadow.camera.near = 10;
    this.sun.shadow.camera.far = 200;
    let emitter = new Mesh(new SphereBufferGeometry(5, 50, 50),
                           new MeshBasicMaterial({color: 0xffd69c}));
    this.sun.add(emitter);
    this.scene.add(this.sun);


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
    let delta = Date.now();
    this.sun.position.x = Math.sin(delta / 1000) * 100;
    this.sun.position.z = Math.cos(delta / 1000) * 100;
    this.renderer.render(this.scene, this.camera);
  }

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
