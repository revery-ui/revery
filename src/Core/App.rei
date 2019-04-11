type t;

type appInitFunc = t => unit;
type idleFunc = unit => unit;

let getWindows: t => list(Window.t);

/** Exit the app */
let quit: (int) => unit;

let createWindow: (~createOptions:Window.windowCreateOptions=?,  t, string) => Window.t;

let start: (~onIdle:idleFunc=?, appInitFunc) => unit;
