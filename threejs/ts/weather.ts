import {
  DirectionalLight,
  DirectionalLightShadow,
  SphereBufferGeometry,
  MeshBasicMaterial,
  Mesh,
  PlaneGeometry,
  TextureLoader,
  MeshLambertMaterial,
  PointsMaterial,
  Vector3,
  Geometry,
  Points,
  AdditiveBlending,
  Scene,
  PerspectiveCamera,
} from "three/src/Three";
import {color_configs, file_urls, tileSize} from "./common";

export interface RainDrop { position: Vector3, velocity: number };

export class WeatherConfigs {
  private isSunny: boolean;
  private sun: DirectionalLight;
  private isCloudy: boolean;
  private clouds: Mesh[];
  private isRainy: boolean;
  private rainGeometry: Geometry;
  private rainDrops: RainDrop[];
  private rain: Points;
  plantConfigs: any;

  constructor(width: number, height: number) {  // SUN
    this.isSunny = false;
    this.sun = new DirectionalLight(color_configs.SUN_LIGHT, 1.5);
    this.sun.name = "sun";
    this.sun.castShadow = true;
    this.sun.position.set(0, 0, 0);
    this.sun.matrixAutoUpdate = true;

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
    for (let i = 0; i < width * height * 3; i++) {
      let cloud = new Mesh(new PlaneGeometry(30, 30), cloudMaterial);
      cloud.name = "cloud";
      cloud.position.set(
          Math.random() * height * tileSize * 1.2 - height / 2 * tileSize * 1.2,
          Math.random() * width * tileSize * 1.2 - width / 2 * tileSize * 1.2,
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
        position: new Vector3(
            Math.random() * height * tileSize - height * tileSize / 2,
            Math.random() * width * tileSize - width * tileSize / 2,
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
  }

  bindingCamera(camera: PerspectiveCamera) {
    this.sun.shadow = new DirectionalLightShadow(camera);
    this.sun.shadow.mapSize.width = 1024;
    this.sun.shadow.mapSize.height = 1024;
    this.sun.shadow.camera.near = 10;
    this.sun.shadow.camera.far = 200;
  }

  renderWeather(camera: PerspectiveCamera) {
    if (this.isSunny) {
      let delta = Date.now();
      this.sun.position.x = Math.sin(delta / 1000) * 100;
      this.sun.position.z = Math.cos(delta / 1000) * 100;
    }
    if (this.isCloudy) {
      this.clouds.forEach(cloud => { cloud.lookAt(camera.position); });
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
  }


  addSuntoScene(scene: Scene) {
    if (this.isSunny) return;
    this.isSunny = true;
    if (this.sun.parent !== scene) {
      scene.add(this.sun);
    }
  }

  removeSunfromScene(scene: Scene) {
    if (!this.isSunny) return;
    this.isSunny = false;
    if (this.sun.parent === scene) {
      scene.remove(this.sun);
    }
  }

  addCloudtoScene(scene: Scene) {
    if (this.isCloudy) return;
    this.isCloudy = true;
    this.clouds.forEach(cloud => {
      if (cloud.parent !== scene) {
        scene.add(cloud);
      }
    });
  }

  removeCloudfromScene(scene: Scene) {
    if (!this.isCloudy) return;
    this.isCloudy = false;
    this.clouds.forEach(cloud => {
      if (cloud.parent === scene) {
        scene.remove(cloud);
      }
    });
  }

  addRaintoScene(scene: Scene) {
    if (this.isRainy) return;
    this.isRainy = true;
    if (this.rain.parent !== scene) {
      scene.add(this.rain);
    }
  }

  removeRainfromScene(scene: Scene) {
    if (!this.isRainy) return;
    this.isRainy = false;
    if (this.rain.parent === scene) {
      scene.remove(this.rain);
    }
  }
}