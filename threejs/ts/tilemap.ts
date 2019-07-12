import {
  Raycaster,
  Vector2,
  Mesh,
  MeshBasicMaterial,
  BoxGeometry,
  Material
} from "three/src/Three";
import {Render} from "./render";
import {color_configs, grid2pos, pos2grid, hydration_configs} from "./common";
import {Configs} from "./config";


export class TileMap {
  tilemap: Mesh[];
  mask: Mesh | undefined;
  updateTile: (gridX: number, gridY: number) => void;

  configs: Configs;
  render: Render;

  // touch event
  dragging: Boolean;
  clientX: number;
  clientY: number;

  constructor(configs: Configs, render: Render,
              updateTile: (gridX: number, gridY: number) => void) {
    this.tilemap = [];
    this.mask = undefined;

    this.configs = configs;
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
      this.render.removefromScene(pop);
      pop.geometry.dispose();
      pop.geometry.dispose();
      pop = undefined;
      pop = this.tilemap.pop();
    }

    for (let i = 0; i < this.configs.getHeight(); i++) {
      for (let j = 0; j < this.configs.getWidth(); j++) {
        let geometry = new BoxGeometry(10, 10, 1);
        let material = new MeshBasicMaterial({
          color: color_configs.TILE_NORMAL,
        });
        let tile = new Mesh(geometry, material);
        tile.position.set(grid2pos(i), grid2pos(j), -0.5);
        tile.receiveShadow = true;
        this.tilemap.push(tile);
        this.render.addtoScene(tile);
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
      let waterlevel = this.configs.getWaterLevel(gridX, gridY);
      for (let material of<Material[]>this.mask.material) {
        if (material instanceof MeshBasicMaterial) {
          material.color.set(hydration_configs[waterlevel]);
        }
      }
      this.mask = undefined;
    }

    if (intersects.length > 0) {
      let tile = intersects[0].object;
      this.mask = <Mesh>tile;
      for (let material of<Material[]>this.mask.material) {
        if (material instanceof MeshBasicMaterial) {
          material.color.set(color_configs.TILE_ONSELECT);
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
      let waterlevel = this.configs.getWaterLevel(gridX, gridY);
      for (let material of<Material[]>this.mask.material) {
        if (material instanceof MeshBasicMaterial) {
          material.color.set(hydration_configs[waterlevel]);
        }
      }
      this.mask = undefined;
    }

    if (intersects.length > 0) {
      let tile = intersects[0].object;
      this.mask = <Mesh>tile;
      for (let material of<Material[]>this.mask.material) {
        if (material instanceof MeshBasicMaterial) {
          material.color.set(color_configs.TILE_ONCLICK);
        }
      }
      let gridX = pos2grid(tile.position.x);
      let gridY = pos2grid(tile.position.y);
      this.updateTile(gridX, gridY);
      let waterlevel = this.configs.getWaterLevel(gridX, gridY);
      if (waterlevel !== undefined) {
        for (let material of<Material[]>this.mask.material) {
          if (material instanceof MeshBasicMaterial) {
            material.color.set(hydration_configs[waterlevel]);
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
