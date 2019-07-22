import {Object3D, Group} from "three/src/Three";

export function clamp(value: number, min: number, max: number) {
  return Math.min(Math.max(value, min), max);
};

export const tileSize = 10;

export interface model { [name: string]: Group; };

export interface Grid {
  planttype: string | undefined;
  plantstatus: number;
  waterlevel: number;
  plantmodel: Object3D | undefined;
};

export interface Env {
  width: number;
  height: number;
  grids: Grid[][];
};

export enum FunctionMode {
  CORN,
  BEAN,
  SQUASH,
  WATER,
  REMOVE,
  HARVEST,
}

export enum WeatherMode {
  SUNNY,
  CLOUDY,
  RAINY,
}

export const file_urls: {
  [name: string]: string;
}
= {
  // texture
  CLOUD: "assets/texture/cloud.png",
  RAINDROP: "assets/texture/raindrop.png",
  TILEMAP: "assets/texture/tilemap.png",
  // image
  NONE: "assets/image/none.png",
  CORN: "assets/image/corn.png",
  SQUASH: "assets/image/squash.png",
};

export interface model_url {
  base_url: string;
  obj_url: string;
  mtl_url: string;
};

export const model_urls: {
  [name: string]: model_url;
}
= {
  // TODO: don't know why magic is required to load first model in the list
  magic: {
    base_url: "assets/model/",
    obj_url: "Corn1.obj",
    mtl_url: "Corn.mtl",
  },
  corn_stage_1: {
    base_url: "assets/model/",
    obj_url: "Corn1.obj",
    mtl_url: "Corn.mtl",
  },
  corn_stage_2: {
    base_url: "assets/model/",
    obj_url: "Corn2.obj",
    mtl_url: "Corn.mtl",
  },
  corn_stage_3: {
    base_url: "assets/model/",
    obj_url: "Corn3.obj",
    mtl_url: "Corn.mtl",
  },
  squash_stage_1: {
    base_url: "assets/model/",
    obj_url: "Squash1.obj",
    mtl_url: "Squash.mtl",
  },
  squash_stage_2: {
    base_url: "assets/model/",
    obj_url: "Squash2.obj",
    mtl_url: "Squash.mtl",
  },
  squash_stage_3: {
    base_url: "assets/model/",
    obj_url: "Squash3.obj",
    mtl_url: "Squash.mtl",
  },
};

export const color_configs: {
  [name: string]: number;
}
= {
  BACKGROUND: 0xaaaaaa,
  AMBIENT_LIGHT: 0xffffff,
  SUN_LIGHT: 0xffd69c,
  RAINDROP: 0x333333,
  GRID: 0xffffff,
  TILE_NORMAL: 0xaf7c52,
  TILE_WATERED1: 0x84532f,
  TILE_WATERED2: 0x843e0a,
  TILE_WATERED3: 0x492205,
  TILE_ONSELECT: 0xffffff,
};

export const hydration_max = 3;

export const hydration_configs: {
  [index: number]: number;
}
= {
  0: color_configs.TILE_NORMAL,
  1: color_configs.TILE_WATERED1,
  2: color_configs.TILE_WATERED2,
  3: color_configs.TILE_WATERED3,
};

export interface plant_status {
  model: string;
  scaleX: number;
  scaleY: number;
  scaleZ: number;
};

export const plant_status_configs: {
  [name: string]: plant_status[];
}
= {
  "CORN": [
    {
      // status 0
      model: "corn_stage_1",
      scaleX: 0.5,
      scaleY: 0.5,
      scaleZ: 0.5,
    },
    {
      // status 1
      model: "corn_stage_1",
      scaleX: 0.75,
      scaleY: 0.75,
      scaleZ: 0.75,
    },
    {
      // status 2
      model: "corn_stage_1",
      scaleX: 1,
      scaleY: 1,
      scaleZ: 1,
    },
    {
      // status 3
      model: "corn_stage_2",
      scaleX: 1,
      scaleY: 1,
      scaleZ: 1,
    },
    {
      // status 4
      model: "corn_stage_2",
      scaleX: 1.25,
      scaleY: 1.25,
      scaleZ: 1.25,
    },
    {
      // status 5
      model: "corn_stage_2",
      scaleX: 1.5,
      scaleY: 1.5,
      scaleZ: 1.5,
    },
    {
      // status 6
      model: "corn_stage_3",
      scaleX: 1.5,
      scaleY: 1.5,
      scaleZ: 1.5,
    },
    {
      // status 7
      model: "corn_stage_3",
      scaleX: 1.75,
      scaleY: 1.75,
      scaleZ: 1.75,
    },
    {
      // status 8
      model: "corn_stage_3",
      scaleX: 2,
      scaleY: 2,
      scaleZ: 2,
    },
  ],
  "SQUASH": [
    {
      // status 0
      model: "squash_stage_1",
      scaleX: 0.5,
      scaleY: 0.5,
      scaleZ: 0.5,
    },
    {
      // status 1
      model: "squash_stage_1",
      scaleX: 0.75,
      scaleY: 0.75,
      scaleZ: 0.75,
    },
    {
      // status 2
      model: "squash_stage_1",
      scaleX: 1,
      scaleY: 1,
      scaleZ: 1,
    },
    {
      // status 3
      model: "squash_stage_2",
      scaleX: 1,
      scaleY: 1,
      scaleZ: 1,
    },
    {
      // status 4
      model: "squash_stage_2",
      scaleX: 1.25,
      scaleY: 1.25,
      scaleZ: 1.25,
    },
    {
      // status 5
      model: "squash_stage_2",
      scaleX: 1.5,
      scaleY: 1.5,
      scaleZ: 1.5,
    },
    {
      // status 6
      model: "squash_stage_3",
      scaleX: 1.5,
      scaleY: 1.5,
      scaleZ: 1.5,
    },
    {
      // status 7
      model: "squash_stage_3",
      scaleX: 1.75,
      scaleY: 1.75,
      scaleZ: 1.75,
    },
    {
      // status 8
      model: "squash_stage_3",
      scaleX: 2,
      scaleY: 2,
      scaleZ: 2,
    },
  ],
};