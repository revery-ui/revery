open Events;

type t;

type size = {
  width: int,
  height: int,
};

type windowRenderCallback = unit => unit;
type windowShouldRenderCallback = unit => bool;
type windowCanQuitCallback = unit => bool;

type callback = unit => unit;
type mouseMoveCallback = mouseMoveEvent => unit;
type keyEventCallback = Key.KeyEvent.t => unit;
type mouseButtonCallback = mouseButtonEvent => unit;
type mouseWheelCallback = mouseWheelEvent => unit;
type textEditCallback = textEditEvent => unit;
type textInputCallback = textInputEvent => unit;
type unsubscribe = unit => unit;

let onFocusGained: (t, callback) => unsubscribe;
let onFocusLost: (t, callback) => unsubscribe;
let onMaximized: (t, callback) => unsubscribe;
let onMinimized: (t, callback) => unsubscribe;
let onRestored: (t, callback) => unsubscribe;
let onCompositionStart: (t, callback) => unsubscribe;
let onCompositionEdit: (t, textEditCallback) => unsubscribe;
let onCompositionEnd: (t, callback) => unsubscribe;
let onExposed: (t, callback) => unsubscribe;
let onKeyDown: (t, keyEventCallback) => unsubscribe;
let onKeyUp: (t, keyEventCallback) => unsubscribe;
let onMouseEnter: (t, callback) => unsubscribe;
let onMouseLeave: (t, callback) => unsubscribe;
let onMouseMove: (t, mouseMoveCallback) => unsubscribe;
let onMouseDown: (t, mouseButtonCallback) => unsubscribe;
let onMouseUp: (t, mouseButtonCallback) => unsubscribe;
let onMouseWheel: (t, mouseWheelCallback) => unsubscribe;
let onTextInputCommit: (t, textInputCallback) => unsubscribe;

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

let setCanQuitCallback: (t, windowCanQuitCallback) => unit;
let setRenderCallback: (t, windowRenderCallback) => unit;
let setShouldRenderCallback: (t, windowShouldRenderCallback) => unit;
