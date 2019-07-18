import {
  Raycaster,
  Vector2,
  Mesh,
  BoxGeometry,
  Material,
  MeshLambertMaterial,
  TextureLoader,
  MultiplyBlending
} from "three/src/Three";
import {Render} from "./render";
import {
  color_configs,
  grid2pos,
  pos2grid,
  hydration_configs,
  file_urls
} from "./common";
import {PlantConfigs} from "./plant";


export class TileMap {
  tilemap: Mesh[];
  mask: Mesh | undefined;
  updateTile: (gridX: number, gridY: number) => void;

  plantConfigs: PlantConfigs;
  render: Render;

  // touch event
  dragging: Boolean;
  clientX: number;
  clientY: number;

  constructor(configs: PlantConfigs, render: Render,
              updateTile: (gridX: number, gridY: number) => void) {
    this.tilemap = [];
    this.mask = undefined;

    this.plantConfigs = configs;
    this.render = render;
    this.updateTile = updateTile;
    this.dragging = false;
    this.clientX = 0;
    this.clientY = 0;
    this.reset();
  }

  reset() {
    let pop = this.tilemap.pop();
    while (pop != undefined) {
      this.render.removeModelfromScene(pop);
      pop.geometry.dispose();
      pop.geometry.dispose();
      pop = undefined;
      pop = this.tilemap.pop();
    }

    for (let i = 0; i < this.plantConfigs.getHeight(); i++) {
      for (let j = 0; j < this.plantConfigs.getWidth(); j++) {
        let geometry = new BoxGeometry(10, 10, 1);
        let material = new MeshLambertMaterial({
          color: color_configs.TILE_NORMAL,
          map: new TextureLoader().load(file_urls.TILEMAP),
          blending: MultiplyBlending,
        });
        let tile = new Mesh(geometry, material);
        tile.position.set(grid2pos(i), grid2pos(j), -0.5);
        tile.receiveShadow = true;
        this.tilemap.push(tile);
        this.render.addModeltoScene(tile);
      }
    }
  }

  onDocumentMouseMove(event: {
    clientX: number;
    clientY: number;
  }) {
    let mouse = new Vector2();
    let raycaster = new Raycaster();
    mouse.set((event.clientX / window.innerWidth) * 2 - 1,
              -(event.clientY / window.innerHeight) * 2 + 1);
    raycaster.setFromCamera(mouse, this.render.getCamera());
    let intersects = raycaster.intersectObjects(this.tilemap);

    if (this.mask != undefined) {
      let gridX = pos2grid(this.mask.position.x);
      let gridY = pos2grid(this.mask.position.y);
      let waterlevel = this.plantConfigs.getWaterLevel(gridX, gridY);
      if (this.mask.material instanceof MeshLambertMaterial) {
        this.mask.material.color.set(hydration_configs[waterlevel]);
      } else {
        for (let material of<Material[]>this.mask.material) {
          if (material instanceof MeshLambertMaterial) {
            material.color.set(hydration_configs[waterlevel]);
          }
        }
      }
      this.mask = undefined;
    }

    if (intersects.length > 0) {
      let tile = intersects[0].object;
      this.mask = <Mesh>tile;
      if (this.mask.material instanceof MeshLambertMaterial) {
        this.mask.material.color.set(color_configs.TILE_ONSELECT);
      } else {
        for (let material of<Material[]>this.mask.material) {
          if (material instanceof MeshLambertMaterial) {
            material.color.set(color_configs.TILE_ONSELECT);
          }
        }
      }
    }
  }

  clickEvent(x: number, y: number) {
    let mouse = new Vector2();
    let raycaster = new Raycaster();
    mouse.set((x / window.innerWidth) * 2 - 1,
              -(y / window.innerHeight) * 2 + 1);
    raycaster.setFromCamera(mouse, this.render.getCamera());
    let intersects = raycaster.intersectObjects(this.tilemap);

    if (this.mask !== undefined) {
      let gridX = pos2grid(this.mask.position.x);
      let gridY = pos2grid(this.mask.position.y);
      let waterlevel = this.plantConfigs.getWaterLevel(gridX, gridY);
      if (this.mask.material instanceof MeshLambertMaterial) {
        this.mask.material.color.set(hydration_configs[waterlevel]);
      } else {
        for (let material of<Material[]>this.mask.material) {
          if (material instanceof MeshLambertMaterial) {
            material.color.set(hydration_configs[waterlevel]);
          }
        }
      }
      this.mask = undefined;
    }

    if (intersects.length > 0) {
      let tile = intersects[0].object;
      this.mask = <Mesh>tile;
      if (this.mask.material instanceof MeshLambertMaterial) {
        this.mask.material.color.set(color_configs.TILE_ONCLICK);
      } else {
        for (let material of<Material[]>this.mask.material) {
          if (material instanceof MeshLambertMaterial) {
            material.color.set(color_configs.TILE_ONCLICK);
          }
        }
      }
      let gridX = pos2grid(tile.position.x);
      let gridY = pos2grid(tile.position.y);
      this.updateTile(gridX, gridY);
      let waterlevel = this.plantConfigs.getWaterLevel(gridX, gridY);
      if (waterlevel !== undefined) {
        if (this.mask.material instanceof MeshLambertMaterial) {
          this.mask.material.color.set(hydration_configs[waterlevel]);
        } else {
          for (let material of<Material[]>this.mask.material) {
            if (material instanceof MeshLambertMaterial) {
              material.color.set(hydration_configs[waterlevel]);
            }
          }
        }
      }
    }
  }

  onDocumentMouseDown(event: MouseEvent) {
    this.clickEvent(event.clientX, event.clientY);
  };

  onDocumentTouchStart(event: TouchEvent) {
    this.dragging = false;
    this.clientX = event.touches[0].clientX;
    this.clientY = event.touches[0].clientY;
  }

  onDocumentTouchMove() { this.dragging = true; }

  onDocumentTouchEnd() {
    if (!this.dragging) {
      this.clickEvent(this.clientX, this.clientY);
    }
  }
}
