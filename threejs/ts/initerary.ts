import {FunctionMode} from "./common";

export class Initerary {
  icon_map: {[mode: string]: string};
  functionMode = FunctionMode.INVESTIGATE;

  constructor() {
    // mapping enum FunctionMode. eg. "BEAN" => "bean-icon"
    this.icon_map = Object.assign(
        {}, ...Object.keys(FunctionMode)
                .filter((key: any) => { return isNaN(Number(key)); })
                .map(mode => ({[mode]: mode.toLowerCase() + "-icon"})));

    let initerary = document.createElement('div');
    initerary.className = "initerary";
    document.body.appendChild(initerary);

    let ul = document.createElement('ul');
    initerary.appendChild(ul);

    Object.values(this.icon_map)
        .forEach(icon => {
          let li = document.createElement('li');
          li.id = icon;
          li.onclick = this.onClick.bind(this);
          ul.appendChild(li);
          let div = document.createElement('div');
          li.appendChild(div);
          let a = document.createElement('a');
          div.appendChild(a);
        });
  }


  onClick(ev: MouseEvent) {
    // unchoose last icon
    let last_id = this.icon_map[FunctionMode[this.functionMode]];
    let last_icon = document.getElementById(last_id);
    if (last_icon instanceof HTMLLIElement) {
      last_icon.style.backgroundColor = "#af7c52";
    }

    // set current icon
    let htmlElement = ev.target as HTMLElement;
    let li = htmlElement.closest('li');
    if (li instanceof HTMLLIElement) {
      li.style.backgroundColor = "red";
      let id = li.id;
      let mode =
          Object.keys(this.icon_map).find(mode => this.icon_map[mode] === id);
      if (typeof mode === 'string') {
        this.functionMode = (<any>FunctionMode)[mode];
      }
    }
  }
}
