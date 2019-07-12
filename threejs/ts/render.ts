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
  FogExp2,
  PlaneGeometry,
  LinearMipMapLinearFilter,
  TextureLoader,
  MeshLambertMaterial,
} from "three/src/Three";
import {OrbitControls} from 'three/examples/jsm/controls/OrbitControls';
import {TileMap} from './tilemap';
import {Configs} from './config';
import {file_urls, color_configs} from "./common";

export class Render {
  private camera: PerspectiveCamera;  // public for convenience
  private scene: Scene;
  private renderer: WebGLRenderer;
  private cameraControls: OrbitControls;
  private sun: DirectionalLight;

  configs: Configs;
  tilemap: TileMap | undefined;
  clouds: Mesh[];

  constructor(configs: Configs) {
    let container = document.createElement('div');
    document.body.appendChild(container);

    // CONFIGS
    this.configs = configs;

    // SCENE
    this.scene = new Scene();
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
        45, window.innerWidth / window.innerHeight, 30, 1000);
    this.camera.position.set(0, -100, 75);

    // CAMERA CONTROLS
    this.cameraControls =
        new OrbitControls(this.camera, this.renderer.domElement);
    this.cameraControls.update();

    // AMBIENT LIGHT
    let ambientlight = new AmbientLight(color_configs.AMBIENT_LIGHT, 0.1);
    this.scene.add(ambientlight);

    // SUN
    this.sun = new DirectionalLight(color_configs.SUN_LIGHT, 1.9);
    this.sun.castShadow = true;
    this.sun.position.set(0, 0, 0);
    this.sun.matrixAutoUpdate = true;
    this.sun.shadow = new DirectionalLightShadow(this.camera);
    this.sun.shadow.mapSize.width = 1024;
    this.sun.shadow.mapSize.height = 1024;
    this.sun.shadow.camera.near = 10;
    this.sun.shadow.camera.far = 200;
    let emitter =
        new Mesh(new SphereBufferGeometry(5, 50, 50),
                 new MeshBasicMaterial({color: color_configs.SUN_LIGHT}));
    this.sun.add(emitter);
    this.scene.add(this.sun);

    // CLOUD
    // let texture = new TextureLoader().load(file_urls.cloud);
    // texture.magFilter = texture.minFilter = LinearMipMapLinearFilter;
    // let cloudMaterial = new MeshLambertMaterial(
    //     {map: texture, transparent: true, alphaTest: 0.1});
    // let cloudGeometry = new Geometry();
    // let plane = new Mesh(new PlaneGeometry(10, 10));
    // for (let i = 0; i < 4000; i++) {
    //   plane.position.set(Math.random() * 200 - 100, Math.random() * 200 -
    //   100,
    //                      Math.random() * 20 + 70);
    //   plane.lookAt(this.camera.position.x, this.camera.position.y,
    //                this.camera.position.z);
    //   plane.scale.x = plane.scale.y = Math.random() * Math.random() * 1.5 +
    //   0.5;
    //   cloudGeometry.mergeMesh(plane);
    // }
    // let cloud = new Mesh(cloudGeometry, cloudMaterial);
    // if (cloud.material instanceof MeshLambertMaterial) {
    //   cloud.material.opacity = 0.3;
    // } else {
    //   for (let material of<MeshLambertMaterial[]>cloud.material) {
    //     material.opacity = 0.3;
    //   }
    // }
    // cloud.castShadow = true;
    // this.scene.add(cloud);

    this.clouds = [];
    let texture = new TextureLoader().load(file_urls.cloud);
    texture.magFilter = texture.minFilter = LinearMipMapLinearFilter;
    let cloudMaterial = new MeshLambertMaterial(
        {map: texture, transparent: true, alphaTest: 0.1, opacity: 0.3});
    for (let i = 0; i < 800; i++) {
      let cloud = new Mesh(new PlaneGeometry(30, 30), cloudMaterial);
      cloud.position.set(Math.random() * 200 - 100, Math.random() * 200 - 100,
                         Math.random() * 10 + 70);
      cloud.scale.x = cloud.scale.y = Math.random() * Math.random() * 1.5 + 0.5;
      cloud.castShadow = true;
      cloud.matrixWorldNeedsUpdate = true;
      this.clouds.push(cloud);
      this.scene.add(cloud);
    }

    // EVENTS
    window.addEventListener('resize', this.onWindowResize.bind(this), false);

    // GRID
    let grid = new GridHelper(100, 10, color_configs.GRID, color_configs.GRID);
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
    this.clouds.forEach(cloud => { cloud.lookAt(this.camera.position); });
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
