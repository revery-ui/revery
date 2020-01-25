open Events;

type t;

type size = {
  width: int,
  height: int,
};

type unsubscribe = unit => unit;

let onFocusGained: (t, unit => unit) => unsubscribe;
let onFocusLost: (t, unit => unit) => unsubscribe;
let onMaximized: (t, unit => unit) => unsubscribe;
let onMinimized: (t, unit => unit) => unsubscribe;
let onRestored: (t, unit => unit) => unsubscribe;
let onCompositionStart: (t, unit => unit) => unsubscribe;
let onCompositionEdit: (t, textEditEvent => unit) => unsubscribe;
let onCompositionEnd: (t, unit => unit) => unsubscribe;
let onExposed: (t, unit => unit) => unsubscribe;
let onKeyDown: (t, Key.KeyEvent.t => unit) => unsubscribe;
let onKeyUp: (t, Key.KeyEvent.t => unit) => unsubscribe;
let onMouseEnter: (t, unit => unit) => unsubscribe;
let onMouseLeave: (t, unit => unit) => unsubscribe;
let onMouseMove: (t, mouseMoveEvent => unit) => unsubscribe;
let onMouseDown: (t, mouseButtonEvent => unit) => unsubscribe;
let onMouseUp: (t, mouseButtonEvent => unit) => unsubscribe;
let onMouseWheel: (t, mouseWheelEvent => unit) => unsubscribe;
let onTextInputCommit: (t, textInputEvent => unit) => unsubscribe;

let canQuit: t => bool;

let getDevicePixelRatio: t => float;
let getFramebufferSize: t => size;
let getRawSize: t => size;
let getScaleAndZoom: t => float;
let getSdlWindow: t => Sdl2.Window.t;
let getZoom: t => float;
let getUniqueId: t => int;

let isDirty: t => bool;
let isTextInputActive: t => bool;

let center: t => unit;
let hide: t => unit;
let show: t => unit;
let maximize: t => unit;
let setBackgroundColor: (t, Color.t) => unit;
let setPosition: (t, int, int) => unit;
let setTitle: (t, string) => unit;
let setZoom: (t, float) => unit;
let setVsync: (t, Vsync.t) => unit;

let render: t => unit;
let handleEvent: (Sdl2.Event.t, t) => unit;

let create: (string, WindowCreateOptions.t) => t;

let takeScreenshot: (t, string) => unit;

let setCanQuitCallback: (t, unit => bool) => unit;
let setRenderCallback: (t, unit => unit) => unit;
let setShouldRenderCallback: (t, unit => bool) => unit;
