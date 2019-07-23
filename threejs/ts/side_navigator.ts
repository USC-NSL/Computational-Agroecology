import {PlantConfigs} from "./plant";
import {Grid, file_urls} from "./common";

export class SideNavigator {
  plantConfigs: PlantConfigs;
  sidenav: HTMLDivElement;
  closebtn: HTMLAnchorElement;
  table: HTMLTableElement;
  image: HTMLImageElement;
  isOpen = false;
  gridInfo = ["plant type", "plant status", "water level"];

  constructor(plantConfigs: PlantConfigs) {
    this.plantConfigs = plantConfigs;

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

    // image
    this.image = document.createElement("img");
    this.image.setAttribute("src", file_urls.NONE);
    this.image.setAttribute("alt", "plant image");
    this.sidenav.appendChild(this.image);

    // table
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

    // resize event
    window.addEventListener('resize', this.onWindowResize.bind(this), false);
  }

  setTableContent(grid: Grid) {
    // plant type
    let pt = document.getElementById(this.gridInfo[0]);
    let planttype = (grid.planttype !== undefined) ? grid.planttype : "NONE";
    if (pt instanceof HTMLTableCellElement) {
      pt.textContent = planttype;
    }
    switch (planttype) {
      case "CORN":
        this.image.setAttribute("src", file_urls.CORN);
        break;
      case "SQUASH":
        this.image.setAttribute("src", file_urls.SQUASH);
        break;
      default:
        this.image.setAttribute("src", file_urls.NONE);
        break;
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
    if (window.innerWidth > 1600) {
      this.sidenav.style.width = "400";
    } else if (window.innerWidth < 1000) {
      this.sidenav.style.width = "250";
    } else {
      this.sidenav.style.width = "25%";
    }
  }

  close() {
    this.isOpen = false;
    this.sidenav.style.width = "0";
  }

  onWindowResize() {
    if (this.isOpen) {
      this.open();
    }
  }
}