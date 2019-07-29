import {FunctionMode} from "./common";

export class Initerary {
  private icon_map: {[mode: string]: string};
  private functionMode = FunctionMode.INVESTIGATE;

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
    this.setFunctionMode(this.functionMode);
  }

  setFunctionMode(functionMode: FunctionMode) {
    let last_icon =
        document.getElementById(this.icon_map[FunctionMode[this.functionMode]]);
    if (last_icon != null) {
      last_icon.style.backgroundColor = "#af7c52";
    }
    let icon =
        document.getElementById(this.icon_map[FunctionMode[functionMode]]);
    if (icon != null) {
      icon.style.backgroundColor = "red";
    }
    this.functionMode = functionMode;
  }

  getFunctionMode(): FunctionMode { return this.functionMode; }

  // TODO: fix handler order
  onClick(ev: MouseEvent) {
    let htmlElement = ev.target as HTMLElement;
    let li = htmlElement.closest('li');
    if (li != null) {
      let id = li.id;
      let mode =
          Object.keys(this.icon_map).find(mode => this.icon_map[mode] === id);
      if (typeof mode === 'string') {
        let functionMode = (<any>FunctionMode)[mode];
        this.setFunctionMode(functionMode);
      }
    }
    ev.stopPropagation();
  }
}
