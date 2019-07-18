import {
  Env,
  model_url,
  model_urls,
  model,
  plant_configs,
  grid2pos,
  clamp,
  hydration_max,
  Grid
} from "./common";
import {Group, Mesh, Object3D} from "three/src/Three";

import {MTLLoader, MaterialCreator} from 'three/examples/jsm/loaders/MTLLoader';
import {OBJLoader} from 'three/examples/jsm/loaders/OBJLoader';

export class PlantConfigs {
  private env: Env;
  private models: model | undefined;

  constructor(width: number, height: number) {
    this.env = {width: width, height: height, grids: []};
    for (var i = 0; i < this.env.height; i++) {
      this.env.grids[i] = [];
      for (var j = 0; j < this.env.width; j++) {
        this.env.grids[i][j] = {
          planttype: undefined,
          plantstatus: 0,
          waterlevel: 0,
          plantmodel: undefined,
        };
      }
    }
  }

  async loadModels() { this.models = await this.loadModelsPromise(); }

  loadModelsPromise(): Promise<model> {
    function loadModel(name: string, object: model_url): Promise<any> {
      const onError = () => console.log("Error: unable to load " + name + ".");

      return new Promise(resolve => {
        var mtlLoader = new MTLLoader();
        mtlLoader.setPath(object.base_url);
        mtlLoader.load(object.mtl_url, (materials: MaterialCreator) => {
          materials.preload();
          for (let key in materials.materials) {
            materials.materials[key].alphaTest = 0.5;
          }
          new OBJLoader()
              .setMaterials(materials)
              .setPath(object.base_url)
              .load(object.obj_url, (object3Ds: Group) => {
                object3Ds.name = name;
                resolve(object3Ds);
              }, undefined, onError);
        }, undefined, onError);
      });
    };

    var promises: Promise<any>[] = [];

    for (var name in model_urls) {
      promises.push(loadModel(name, model_urls[name]));
    }

    return new Promise(resolve => {
      Promise.all(promises).then(models => resolve(Object.assign(
                                     {}, ...models.map(s => ({[s.name]: s})))));
    })
  }

  // reset renderer first
  reset() {
    for (var i = 0; i < this.env.height; i++) {
      for (var j = 0; j < this.env.width; j++) {
        this.env.grids[i][j].planttype = undefined;
        this.env.grids[i][j].plantstatus = 0;
        this.env.grids[i][j].waterlevel = 0;
        this.env.grids[i][j].plantmodel = undefined;
      }
    }
  }

  getHeight(): number { return this.env.height; }
  getWidth(): number { return this.env.width; }

  getGrid(gridX: number, gridY: number): Grid {
    return this.env.grids[gridX][gridY];
  }

  getPlantType(gridX: number, gridY: number): string | undefined {
    return this.env.grids[gridX][gridY].planttype;
  }

  setPlantType(gridX: number, gridY: number, planttype: string | undefined) {
    this.env.grids[gridX][gridY].planttype = planttype;
  }

  getPlantStatus(gridX: number, gridY: number): number {
    return this.env.grids[gridX][gridY].plantstatus;
  }

  setPlantStatus(gridX: number, gridY: number, plantstatus: number) {
    this.env.grids[gridX][gridY].plantstatus = plantstatus;
  }

  getWaterLevel(gridX: number, gridY: number): number {
    return this.env.grids[gridX][gridY].waterlevel;
  }

  setWaterLevel(gridX: number, gridY: number, waterlevel: number) {
    this.env.grids[gridX][gridY].waterlevel =
        clamp(waterlevel, 0, hydration_max);
  }

  water(gridX: number, gridY: number): number {
    this.setWaterLevel(gridX, gridY,
                       this.env.grids[gridX][gridY].waterlevel + 1);
    return this.env.grids[gridX][gridY].waterlevel;
  }

  addPlantModel(planttype: string, plantstatus: number, gridX: number,
                gridY: number): Object3D
      | undefined {
    if (this.env.grids[gridX][gridY].plantmodel != undefined) {
      console.log("connot plant " + planttype + " in occupied tile.");
      return undefined;
    } else {
      if (plantstatus >= plant_configs[planttype].length)
        plantstatus = plant_configs[planttype].length - 1;

      var configs = plant_configs[planttype][plantstatus];

      if (this.models === undefined) {
        console.log("Models are not loaded yet.");
        return undefined;
      }
      var model = this.models[configs.model].clone();
      model.scale.set(configs.scaleX, configs.scaleY, configs.scaleZ);
      model.position.set(grid2pos(gridX), grid2pos(gridY), 0);
      model.castShadow = true;
      model.receiveShadow = true;

      this.env.grids[gridX][gridY].planttype = planttype;
      this.env.grids[gridX][gridY].plantstatus = plantstatus;
      this.env.grids[gridX][gridY].plantmodel = model;
      return model;
    }
  }

  removePlantModel(gridX: number, gridY: number): Object3D | undefined {
    let plantmodel = this.env.grids[gridX][gridY].plantmodel;
    if (plantmodel !== undefined) {
      for (let mesh of<Mesh[]>plantmodel.children) {
        mesh.geometry.dispose();
      }
      this.env.grids[gridX][gridY].planttype = undefined;
      this.env.grids[gridX][gridY].plantstatus = 0;
      this.env.grids[gridX][gridY].plantmodel = undefined;
    }
    return plantmodel;
  }
}