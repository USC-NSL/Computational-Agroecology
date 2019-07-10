import {Raycaster, Vector2, Mesh, MeshBasicMaterial, BoxGeometry} from "Three";
import {Render} from "./render";
import {
  tilemap_color_configs,
  grid2pos,
  pos2grid,
  hydration_configs
} from "./common";
import {Configs} from "./config";


export class TileMap {
  tilemap: Mesh[];
  mask: Mesh | undefined;
  updateTile: (gridX: number, gridY: number) => void;

  configs: Configs;
  render: Render;
  constructor(configs: Configs, render: Render,
              updateTile: (gridX: number, gridY: number) => void) {
    this.tilemap = [];
    this.mask = undefined;

    this.configs = configs;
    this.render = render;
    this.updateTile = updateTile;
    this.reset();
  };

  reset() {
    let pop = this.tilemap.pop();
    while (pop != undefined) {
      this.render.removefromScene(pop);
      pop.geometry.dispose();
      pop.geometry.dispose();
      pop = undefined;
      pop = this.tilemap.pop();
    }

    for (var i = 0; i < this.configs.getHeight(); i++) {
      for (var j = 0; j < this.configs.getWidth(); j++) {
        var geometry = new BoxGeometry(10, 10, 1);
        var material = new MeshBasicMaterial({
          color: tilemap_color_configs.NORMAL,
        });
        var tile: Mesh = new Mesh(geometry, material);
        // tile.material.needsUpdate = true;
        tile.position.set(grid2pos(i), grid2pos(j), -0.5);
        this.tilemap.push(tile);
        this.render.addtoScene(tile);
      }
    }
  };

  onDocumentMouseMove(event: {
    clientX: number;
    clientY: number;
  }) {
    // event.preventDefault();
    var mouse = new Vector2();
    var raycaster = new Raycaster();
    mouse.set((event.clientX / window.innerWidth) * 2 - 1,
              -(event.clientY / window.innerHeight) * 2 + 1);
    raycaster.setFromCamera(mouse, this.render.getCamera());
    // calculate objects intersecting the picking ray
    var intersects = raycaster.intersectObjects(this.tilemap);

    if (this.mask != undefined) {
      var gridX = pos2grid(this.mask.position.x);
      var gridY = pos2grid(this.mask.position.y);
      var waterlevel = this.configs.getWaterLevel(gridX, gridY);
      if (this.mask.material instanceof MeshBasicMaterial) {
        this.mask.material.color.set(hydration_configs[waterlevel]);
      } else {
        for (let material of<MeshBasicMaterial[]>this.mask.material) {
          material.color.set(hydration_configs[waterlevel]);
        }
      }
      this.mask = undefined;
    }

    if (intersects.length > 0) {
      var tile = intersects[0].object;
      this.mask = <Mesh>tile;
      if (this.mask.material instanceof MeshBasicMaterial) {
        this.mask.material.color.set(tilemap_color_configs.ONSELECT);
      } else {
        for (let material of<MeshBasicMaterial[]>this.mask.material) {
          material.color.set(tilemap_color_configs.ONSELECT);
        }
      }
    }
    this.render.render();
  };

  onDocumentMouseDown(event: {
    clientX: number;
    clientY: number;
  }) {
    // event.preventDefault();
    var mouse = new Vector2();
    var raycaster = new Raycaster();
    mouse.set((event.clientX / window.innerWidth) * 2 - 1,
              -(event.clientY / window.innerHeight) * 2 + 1);
    raycaster.setFromCamera(mouse, this.render.getCamera());
    // calculate objects intersecting the picking ray
    var intersects = raycaster.intersectObjects(this.tilemap);

    if (this.mask != undefined) {
      var gridX = pos2grid(this.mask.position.x);
      var gridY = pos2grid(this.mask.position.y);
      var waterlevel = this.configs.getWaterLevel(gridX, gridY);
      if (this.mask.material instanceof MeshBasicMaterial) {
        this.mask.material.color.set(hydration_configs[waterlevel]);
      } else {
        for (let material of<MeshBasicMaterial[]>this.mask.material) {
          material.color.set(hydration_configs[waterlevel]);
        }
      }
      this.mask = undefined;
    }

    if (intersects.length > 0) {
      var tile = intersects[0].object;
      this.mask = <Mesh>tile;
      if (this.mask.material instanceof MeshBasicMaterial) {
        this.mask.material.color.set(tilemap_color_configs.CONFIRMED);
      } else {
        for (let material of<MeshBasicMaterial[]>this.mask.material) {
          material.color.set(tilemap_color_configs.CONFIRMED);
        }
      }
      var gridX = pos2grid(tile.position.x);
      var gridY = pos2grid(tile.position.y);
      this.updateTile(gridX, gridY);
      var waterlevel = this.configs.getWaterLevel(gridX, gridY);
      if (waterlevel !== undefined) {
        if (this.mask.material instanceof MeshBasicMaterial) {
          this.mask.material.color.set(hydration_configs[waterlevel]);
        } else {
          for (let material of<MeshBasicMaterial[]>this.mask.material) {
            material.color.set(hydration_configs[waterlevel]);
          }
        }
      }
    }
    this.render.render();
  };
}
