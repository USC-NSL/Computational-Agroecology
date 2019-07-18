import {
  Scene,
  PerspectiveCamera,
  WebGLRenderer,
  Object3D,
  Color,
  Camera,
  DirectionalLight,
  PCFSoftShadowMap,
  DirectionalLightShadow,
  AmbientLight,
  SphereBufferGeometry,
  MeshBasicMaterial,
  Mesh,
  FogExp2,
  PlaneGeometry,
  TextureLoader,
  MeshLambertMaterial,
  PointsMaterial,
  Vector3,
  Geometry,
  Points,
  AdditiveBlending,
} from "three/src/Three";
import {MapControls} from 'three/examples/jsm/controls/MapControls';

import {TileMap} from './tilemap';
import {PlantConfigs} from './plant';
import {file_urls, color_configs} from "./common";

export interface RainDrop { position: Vector3, velocity: number };

export class Render {
  private camera: PerspectiveCamera;  // public for convenience
  private scene: Scene;
  private renderer: WebGLRenderer;
  private mapControl: MapControls;

  private isSunny: boolean;
  private sun: DirectionalLight;
  private isCloudy: boolean;
  private clouds: Mesh[];
  private isRainy: boolean;
  private rainGeometry: Geometry;
  private rainDrops: RainDrop[];
  private rain: Points;

  plantConfigs: PlantConfigs;
  tilemap: TileMap | undefined;

  constructor(configs: PlantConfigs) {
    let container = document.createElement('div');
    container.setAttribute("id", "threejs");
    document.body.appendChild(container);

    // CONFIGS
    this.plantConfigs = configs;

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

    // SUN
    this.isSunny = false;
    this.sun = new DirectionalLight(color_configs.SUN_LIGHT, 1.5);
    this.sun.name = "sun";
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

    // CLOUDS
    this.isCloudy = false;
    this.clouds = [];
    let cloudMaterial = new MeshLambertMaterial({
      map: new TextureLoader().load(file_urls.CLOUD),
      transparent: true,
      alphaTest: 0.1,
      opacity: 0.3
    });
    let width = this.plantConfigs.getWidth();
    let height = this.plantConfigs.getWidth();
    for (let i = 0; i < width * height * 3; i++) {
      let cloud = new Mesh(new PlaneGeometry(30, 30), cloudMaterial);
      cloud.name = "cloud";
      cloud.position.set(
          Math.random() * width * 10 * 1.2 - width / 2 * 10 * 1.2,
          Math.random() * height * 10 * 1.2 - height / 2 * 10 * 1.2,
          Math.random() * 10 + 70);
      cloud.scale.x = cloud.scale.y = Math.random() * Math.random() * 1.5 + 0.5;
      cloud.castShadow = true;
      cloud.matrixWorldNeedsUpdate = true;
      this.clouds.push(cloud);
    }


    // RAIN
    this.isRainy = false;
    this.rainDrops = [];
    this.rainGeometry = new Geometry();
    for (let i = 0; i < width * height * 3; i++) {
      let rainDrop: RainDrop = {
        position: new Vector3(Math.random() * width * 10 - width * 10 / 2,
                              Math.random() * height * 10 - height * 10 / 2,
                              Math.random() * 70),
        velocity: -Math.random() * Math.random() * 1.5,
      };
      this.rainGeometry.vertices.push(rainDrop.position);
      this.rainDrops.push(rainDrop);
    }
    let rainMaterial = new PointsMaterial({
      color: color_configs.RAINDROP,
      size: 1.5,
      map: new TextureLoader().load(file_urls.RAINDROP),
      blending: AdditiveBlending,
      depthTest: false,
      transparent: true
    });
    this.rain = new Points(this.rainGeometry, rainMaterial);
    this.rain.name = "rain";

    // EVENTS
    window.addEventListener('resize', this.onWindowResize.bind(this), false);

    this.animate();
  }

  getCamera(): Camera { return this.camera; }

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
    if (this.isSunny) {
      let delta = Date.now();
      this.sun.position.x = Math.sin(delta / 1000) * 100;
      this.sun.position.z = Math.cos(delta / 1000) * 100;
    }
    if (this.isCloudy) {
      this.clouds.forEach(cloud => { cloud.lookAt(this.camera.position); });
    }
    if (this.isRainy) {
      this.rainDrops.forEach(rainDrop => {
        rainDrop.velocity -= Math.random() * 0.05;
        rainDrop.position.z += rainDrop.velocity;
        if (rainDrop.position.z < 0) {
          rainDrop.position.z = 70;
          rainDrop.velocity = -Math.random() * 0.5;
        }
      });
      this.rainGeometry.verticesNeedUpdate = true;
    }
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

  addSuntoScene() {
    if (this.isSunny) return;
    this.isSunny = true;
    if (this.sun.parent !== this.scene) {
      this.scene.add(this.sun);
    }
  }

  removeSunfromScene() {
    if (!this.isSunny) return;
    this.isSunny = false;
    if (this.sun.parent === this.scene) {
      this.scene.remove(this.sun);
    }
  }

  addCloudtoScene() {
    if (this.isCloudy) return;
    this.isCloudy = true;
    this.clouds.forEach(cloud => {
      if (cloud.parent !== this.scene) {
        this.scene.add(cloud);
      }
    });
  }

  removeCloudfromScene() {
    if (!this.isCloudy) return;
    this.isCloudy = false;
    this.clouds.forEach(cloud => {
      if (cloud.parent === this.scene) {
        this.scene.remove(cloud);
      }
    });
  }

  addRaintoScene() {
    if (this.isRainy) return;
    this.isRainy = true;
    if (this.rain.parent !== this.scene) {
      this.scene.add(this.rain);
    }
  }

  removeRainfromScene() {
    if (!this.isRainy) return;
    this.isRainy = false;
    if (this.rain.parent === this.scene) {
      this.scene.remove(this.rain);
    }
  }
}
