open Events;

type t;

type size = {
  width: int,
  height: int,
};

type unsubscribe = unit => unit;

let onBeforeRender: (t, unit => unit) => unsubscribe;
let onAfterRender: (t, unit => unit) => unsubscribe;
let onBeforeSwap: (t, unit => unit) => unsubscribe;
let onAfterSwap: (t, unit => unit) => unsubscribe;
let onFocusGained: (t, unit => unit) => unsubscribe;
let onFocusLost: (t, unit => unit) => unsubscribe;
let onMaximized: (t, unit => unit) => unsubscribe;
let onFullscreen: (t, unit => unit) => unsubscribe;
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
let onSizeChanged: (t, size => unit) => unsubscribe;
let onMoved: (t, ((int, int)) => unit) => unsubscribe;
let onTextInputCommit: (t, textInputEvent => unit) => unsubscribe;
let onFileDropped: (t, fileDropEvent => unit) => unsubscribe;

let canQuit: t => bool;

let getDevicePixelRatio: t => float;

/**
  [getSize(window)] returns a [size] describing the window dimensions, accounting for display scaling.
*/
let getSize: t => size;

let getFramebufferSize: t => size;

/**
  [setSize(~width, ~height, window)] sets the window size, taking display scaling into account.

  For example, for a Windows display with 200% scaling, [setSize(~width=400, ~height=300, window)],
  will actually set the window to a height of 800 pixels wide by 600 pixels high. This is usually
  the behavior you want. To directly set the size, without considering display scaling, use [setUnscaledSize]
*/
let setSize: (~width: int, ~height: int, t) => unit;

/**
  [setMinimumSize(~width, ~height, window)] sets the minimum window size.
*/
let setMinimumSize: (~width: int, ~height: int, t) => unit;

let getPosition: t => (int, int);
let getScaleAndZoom: t => float;
let getSdlWindow: t => Sdl2.Window.t;
let getZoom: t => float;
let getUniqueId: t => int;
let getBackgroundColor: t => Color.t;

let isDirty: t => bool;

let isDecorated: t => bool;

let setInputRect: (t, int, int, int, int) => unit;
let startTextInput: t => unit;
let stopTextInput: t => unit;
let isTextInputActive: t => bool;

let center: t => unit;
let hide: t => unit;
let show: t => unit;
let maximize: t => unit;
let isMaximized: t => bool;
let isFullscreen: t => bool;
let raise: t => unit;
let restore: t => unit;
let minimize: t => unit;

let setTransparency: (t, float) => unit;
let setBackgroundColor: (t, Color.t) => unit;
let setPosition: (t, int, int) => unit;
let setTitle: (t, string) => unit;
let setZoom: (t, float) => unit;
let setVsync: (t, Vsync.t) => unit;

let render: t => unit;
let handleEvent: (Sdl2.Event.t, t) => unit;

/**
  [create(name, options)] creates a new Revery application window.

  See [WindowCreateOptions] for a list of available options.
*/
let create: (string, WindowCreateOptions.t) => t;

let takeScreenshot: (t, string) => unit;

let setCanQuitCallback: (t, unit => bool) => unit;
let setRenderCallback: (t, unit => unit) => unit;
let setShouldRenderCallback: (t, unit => bool) => unit;

let getFPS: t => int;
let showFPSCounter: t => unit;
let hideFPSCounter: t => unit;
let shouldShowFPSCounter: t => bool;
