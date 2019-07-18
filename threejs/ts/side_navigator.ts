import {PlantConfigs} from "./plant";
import {Grid} from "./common";

export class SideNavigator {
  plantConfigs: PlantConfigs;
  sidenav: HTMLDivElement;
  closebtn: HTMLAnchorElement;
  table: HTMLTableElement;
  isOpen: boolean;
  gridInfo = ["plant type", "plant status", "water level"];

  constructor(plantConfigs: PlantConfigs) {
    this.plantConfigs = plantConfigs;
    this.isOpen = false;

    // side navigator
    this.sidenav = document.createElement('div');
    this.sidenav.className = "sidenav";
    document.body.appendChild(this.sidenav);

    // close button
    this.closebtn = document.createElement('a');
    let textnode = document.createTextNode("\xD7");
    this.closebtn.appendChild(textnode);
    this.closebtn.href = "javascript:void(0)";
    this.closebtn.className = "closebtn";
    this.closebtn.onclick = this.close.bind(this);
    this.sidenav.appendChild(this.closebtn);

    // content
    this.table = document.createElement("table");
    this.gridInfo.forEach(name => {
      let tr = document.createElement("tr");
      let th = document.createElement("th");
      th.textContent = name + ":";
      let td = document.createElement("td");
      td.id = name;
      tr.appendChild(th);
      tr.appendChild(td);
      this.table.appendChild(tr);
    });
    this.sidenav.appendChild(this.table);
  }

  setTableContent(grid: Grid) {
    // plant type
    let pt = document.getElementById(this.gridInfo[0]);
    if (pt instanceof HTMLTableCellElement) {
      pt.textContent = (grid.planttype !== undefined) ? grid.planttype : "none";
    }
    // plant status
    let ps = document.getElementById(this.gridInfo[1]);
    if (ps instanceof HTMLTableCellElement) {
      ps.textContent = grid.plantstatus.toString();
    }
    // water level
    let wl = document.getElementById(this.gridInfo[2]);
    if (wl instanceof HTMLTableCellElement) {
      wl.textContent = grid.waterlevel.toString();
    }
  }

  open() {
    this.isOpen = true;
    this.sidenav.style.width = "25%";
  }

  close() {
    this.isOpen = false;
    this.sidenav.style.width = "0";
  }
}