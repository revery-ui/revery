open Revery;

let init = (app) => {
    let _ = app#createWindow("test");
};

/* Js.Unsafe.fun_call(Js.Unsafe.js_expr("console.log"), [||]); */

App.start(init);
