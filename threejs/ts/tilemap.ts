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
import {color_configs, hydration_configs, file_urls, tileSize} from "./common";
import {PlantConfigs} from "./plant";
import {SideNavigator} from "./side_navigator";


export class TileMap {
  tilemap: Mesh[] = [];
  mask: Mesh | undefined = undefined;
  updateTile: (gridX: number, gridY: number) => void;

  // modules
  plantConfigs: PlantConfigs;
  render: Render;
  sideNavigator: SideNavigator;

  // mouse and touch event
  isDragging = false;

  constructor(plantConfigs: PlantConfigs, render: Render,
              sideNavigator: SideNavigator,
              updateTile: (gridX: number, gridY: number) => void) {
    this.plantConfigs = plantConfigs;
    this.render = render;
    this.sideNavigator = sideNavigator;
    this.updateTile = updateTile;
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
        tile.position.set(this.gridX2posX(i), this.gridY2posY(j), -0.5);
        tile.receiveShadow = true;
        this.tilemap.push(tile);
        this.render.addModeltoScene(tile);
      }
    }
  }

  onDocumentMouseMove(event: MouseEvent) {
    this.isDragging = true;

    let mouse = new Vector2();
    let raycaster = new Raycaster();
    mouse.set((event.clientX / window.innerWidth) * 2 - 1,
              -(event.clientY / window.innerHeight) * 2 + 1);
    raycaster.setFromCamera(mouse, this.render.camera);
    let intersects = raycaster.intersectObjects(this.tilemap);

    if (this.mask != undefined) {
      let gridX = this.posX2gridX(this.mask.position.x);
      let gridY = this.posY2gridY(this.mask.position.y);
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

  leftClickEvent(x: number, y: number) {
    let mouse = new Vector2();
    let raycaster = new Raycaster();
    mouse.set((x / window.innerWidth) * 2 - 1,
              -(y / window.innerHeight) * 2 + 1);
    raycaster.setFromCamera(mouse, this.render.camera);
    let intersects = raycaster.intersectObjects(this.tilemap);

    if (this.mask !== undefined) {
      let gridX = this.posX2gridX(this.mask.position.x);
      let gridY = this.posY2gridY(this.mask.position.y);
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
      let gridX = this.posX2gridX(tile.position.x);
      let gridY = this.posY2gridY(tile.position.y);

      if (this.sideNavigator.isOpen) {
        // update display info
        this.sideNavigator.setTableContent(
            this.plantConfigs.getGrid(gridX, gridY));
      } else {
        // update tile
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
    } else {
      this.sideNavigator.close();
    }
  }

  rightClickEvent(x: number, y: number) {
    let mouse = new Vector2();
    let raycaster = new Raycaster();
    mouse.set((x / window.innerWidth) * 2 - 1,
              -(y / window.innerHeight) * 2 + 1);
    raycaster.setFromCamera(mouse, this.render.camera);
    let intersects = raycaster.intersectObjects(this.tilemap);

    if (this.mask !== undefined) {
      let gridX = this.posX2gridX(this.mask.position.x);
      let gridY = this.posY2gridY(this.mask.position.y);
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
      let gridX = this.posX2gridX(tile.position.x);
      let gridY = this.posY2gridY(tile.position.y);
      this.sideNavigator.setTableContent(
          this.plantConfigs.getGrid(gridX, gridY));
      this.sideNavigator.open();
    } else {
      this.sideNavigator.close();
    }
  }

  onDocumentMouseDown() { this.isDragging = false; };

  onDocumentMouseUp(event: MouseEvent) {
    if (!this.isDragging) {
      switch (event.which) {
        case 1:
          this.leftClickEvent(event.clientX, event.clientY);
          break;
        case 3:
          this.rightClickEvent(event.clientX, event.clientY);
          break;
        default:
          break;
      }
    }
  };

  onDocumentTouchStart() { this.isDragging = false; }
  onDocumentTouchMove() { this.isDragging = true; }
  onDocumentTouchEnd(event: TouchEvent) {
    if (!this.isDragging) {
      this.leftClickEvent(event.touches[0].clientX, event.touches[0].clientY);
    }
  }

  posX2gridX(posX: number) {
    return posX / tileSize + this.plantConfigs.getHeight() / 2 - 0.5;
  }
  posY2gridY(posY: number) {
    return posY / tileSize + this.plantConfigs.getWidth() / 2 - 0.5;
  }
  gridX2posX(gridX: number) {
    return tileSize * (-this.plantConfigs.getHeight() / 2 + 0.5 + gridX);
  }
  gridY2posY(gridY: number) {
    return tileSize * (-this.plantConfigs.getWidth() / 2 + 0.5 + gridY);
  }
}
