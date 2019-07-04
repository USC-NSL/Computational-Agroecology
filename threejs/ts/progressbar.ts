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


// var LoadingWrapper = function (fn) {
//     return async function () {
//         progressbar.show();
//         console.log("load finished");
//         var result = await new Promise(resolve => {
//             result = fn.apply(this, arguments);
//             console.log(result);
//             resolve(result);
//         });
//         console.log(result);
//         progressbar.hide();
//         return result;
//     }
// };