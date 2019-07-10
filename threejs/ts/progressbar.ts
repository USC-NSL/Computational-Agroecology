let progressbar = new class {
  bar: HTMLElement | null;
  constructor() { this.bar = document.getElementById('loader'); };
  show() {
    if (this.bar !== null) this.bar.style.display = "block";
  };
  hide() {
    if (this.bar !== null) this.bar.style.display = "block";
  };
}
();

progressbar.hide();
