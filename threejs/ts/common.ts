import {Object3D, Group} from "Three";

export function clamp(value: number, min: number, max: number) {
  return Math.min(Math.max(value, min), max);
};

export function pos2grid(pos: number) {
  return (pos + 45) / 10;
};
export function grid2pos(grid: number) {
  return -45 + 10 * grid;
};

export interface model { [name: string]: Group; }

export interface Grid {
  planttype: string | undefined;
  plantstatus: number;
  waterlevel: number;
  plantmodel: Object3D | undefined;
}

export interface Env {
  width: number;
  height: number;
  grids: Grid[][];
}

export enum Mode {
  CORN,
  BEAN,
  SQUASH,
  WATER,
  REMOVE,
  HARVEST,
}
;

export interface model_url {
  base_url: string;
  obj_url: string;
  mtl_url: string;
}

export interface model_urls { [name: string]: model_url; }

export const loader_configs: model_urls = {
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

export interface tilemap_color { [name: string]: number; }

export const tilemap_color_configs: tilemap_color = {
  NORMAL: 0xaf7c52,
  WATERED1: 0x84532f,
  WATERED2: 0x843e0a,
  WATERED3: 0x492205,
  ONSELECT: 0xffffff,
  CONFIRMED: 0xff0000,
};

export const hydration_max = 3;

export interface hydration { [index: number]: number; }

export const hydration_configs: hydration = {
  0: tilemap_color_configs.NORMAL,
  1: tilemap_color_configs.WATERED1,
  2: tilemap_color_configs.WATERED2,
  3: tilemap_color_configs.WATERED3,
};

export interface plant_status {
  model: string;
  scaleX: number;
  scaleY: number;
  scaleZ: number;
}

export interface plants { [name: string]: plant_status[]; }

export const plant_configs: plants = {
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