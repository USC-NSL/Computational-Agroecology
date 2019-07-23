import {FunctionMode} from "./common";

export class Initerary {
  icons = [
    "investigate-icon",
    "corn-icon",
    "bean-icon",
    "squash-icon",
    "water-icon",
    "remove-icon",
    "harvest-icon"
  ];
  functionMode = FunctionMode.INVESTIGATE;
  functionMap: {[icon: string]: string} = {};

  constructor() {
    let initerary = document.createElement('div');
    initerary.className = "initerary";
    document.body.appendChild(initerary);

    let ul = document.createElement('ul');
    ul.id = "navlist";
    initerary.appendChild(ul);

    this.icons.forEach(icon => {
      let li = document.createElement('li');
      li.id = icon;
      ul.appendChild(li);
      let a = document.createElement('a');
      li.appendChild(a);
    });
  }
}
