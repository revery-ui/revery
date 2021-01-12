module Float32Array = Float32Array;
module Uint16Array = Uint16Array;

module Size = {
  type t = {
    width: int,
    height: int,
  };
};

module Rect = {
  type t = {
    x: int,
    y: int,
    width: int,
    height: int,
  };

  let toString = ({x, y, width, height}) =>
    Printf.sprintf("x: %d y: %d width: %d height: %d", x, y, width, height);
};

module ScreenSaver = {
  external enable: unit => unit = "resdl_SDL_EnableScreenSaver";
  external disable: unit => unit = "resdl_SDL_DisableScreenSaver";
  external isEnabled: unit => bool = "resdl_SDL_IsScreenSaverEnabled";
};

module Surface = {
  type t;
  external createFromImagePath: string => result(t, string) =
    "resdl_SDL_CreateRGBSurfaceFromImage";
};

module Audio = {
  module Buffer = {
    type t;
  };
  module Spec = {
    type t = {
      freq: int,
      format: int,
      channels: int,
      silence: int,
      samples: int,
      padding: int,
      size: int,
    };
  };
  module Wav = {
    external load: string => result((Spec.t, Buffer.t, int), string) =
      "resdl_SDL_LoadWAV";
  };
  module Device = {
    type t;
    external open_: Spec.t => t = "resdl_SDL_OpenAudioDevice";
    external close: t => unit = "resdl_SDL_CloseAudioDevice";
    external pause: (t, bool) => unit = "resdl_SDL_PauseAudioDevice";
  };
  external queue: (Device.t, Buffer.t, int) => result(unit, string) =
    "resdl_SDL_QueueAudio";
  external clearQueued: Device.t => unit = "resdl_SDL_ClearQueuedAudio";
};

module Clipboard = {
  external getText: unit => option(string) = "resdl_SDL_GetClipboardText";
  external setText: string => unit = "resdl_SDL_SetClipboardText";
  external hasText: unit => bool = "resdl_SDL_HasClipboardText";
};

module PixelFormat = {
  type t;

  external toString: t => string = "resdl_SDL_GetPixelFormatName";
};

module Display = {
  type t;

  module Dpi = {
    type t = {
      ddpi: float,
      hdpi: float,
      vdpi: float,
    };

    let show = (v: t) => {
      Printf.sprintf("ddpi: %f hdpi: %f vdpi: %f", v.ddpi, v.hdpi, v.vdpi);
    };
  };

  module Mode = {
    type t = {
      pixelFormat: PixelFormat.t,
      width: int,
      height: int,
      refreshRate: int,
    };

    let show = (v: t) => {
      Printf.sprintf(
        "pixelFormat: %s width: %d height: %d refreshRate: %d",
        v.pixelFormat |> PixelFormat.toString,
        v.width,
        v.height,
        v.refreshRate,
      );
    };
  };

  external getNumberOfDisplays: unit => int = "resdl_SDL_GetNumVideoDisplays";
  let getDisplays = () =>
    List.init(getNumberOfDisplays(), (i) => (Obj.magic(i: int): t));

  external getDPI: t => Dpi.t = "resdl_SDL_GetDisplayDPI";
  external getCurrentMode: t => Mode.t = "resdl_SDL_GetCurrentDisplayMode";
  external getDesktopMode: t => Mode.t = "resdl_SDL_GetDesktopDisplayMode";
  external getName: t => string = "resdl_SDL_GetDisplayName";
  external getBounds: t => Rect.t = "resdl_SDL_GetDisplayBounds";
  external getUsableBounds: t => Rect.t = "resdl_SDL_GetDisplayUsableBounds";
};

module Log = {
  type category =
    | Application
    | Error
    | Assert
    | System
    | Audio
    | Video
    | Render
    | Input
    | Test
    | Custom
    | Unknown;

  type priority =
    | Verbose
    | Debug
    | Info
    | Warn
    | Error
    | Critical;

  type logOutputFunction = (category, priority, string) => unit;

  let _outputFunction: ref(logOutputFunction) = ref((_, _, _) => ());
  let setOutputFunction = outputFunction => {
    _outputFunction := outputFunction;
  };

  let _onLog = (category, priority, str) => {
    _outputFunction^(category, priority, str);
  };

  Callback.register("reason_sdl2_onLog", _onLog);
};

module Platform = {
  external getName: unit => string = "resdl_SDL_GetPlatform";

  external getVersion: unit => string = "resdl_SDL_GetVersion";

  // Windows only
  external win32AllocConsole: unit => int = "resdl_SDL_WinAllocConsole";
  external win32AttachConsole: unit => int = "resdl_SDL_WinAttachConsole";
};

module Window = {
  type t;

  type hitTestResult =
    | Normal
    | Draggable
    | ResizeTopLeft
    | ResizeTop
    | ResizeTopRight
    | ResizeRight
    | ResizeBottomRight
    | ResizeBottom
    | ResizeBottomLeft
    | ResizeLeft;

  type hitTestCallback = (t, int, int) => hitTestResult;

  external create:
    (
      string,
      [ | `Undefined | `Centered | `Absolute(int)],
      [ | `Undefined | `Centered | `Absolute(int)],
      int,
      int,
      [ | `Auto | `ForceHardware | `ForceSoftware]
    ) =>
    t =
    "resdl_SDL_CreateWindow_byte" "resdl_SDL_CreateWindow";
  external getId: t => int = "resdl_SDL_GetWindowId";
  external getSize: t => Size.t = "resdl_SDL_GetWindowSize";
  external getPosition: t => (int, int) = "resdl_SDL_GetWindowPosition";
  external setBordered: (t, bool) => unit = "resdl_SDL_SetWindowBordered";
  external getPixelFormat: t => PixelFormat.t =
    "resdl_SDL_GetWindowPixelFormat";
  external setIcon: (t, Surface.t) => unit = "resdl_SDL_SetWindowIcon";
  external setTransparency: (t, float) => unit =
    "resdl_SDL_SetWindowTransparency";
  external setPosition: (t, int, int) => unit = "resdl_SDL_SetWindowPosition";
  external center: t => unit = "resdl_SDL_WindowCenter";
  external setResizable: (t, bool) => unit = "resdl_SDL_SetWindowResizable";
  external setSize: (t, int, int) => unit = "resdl_SDL_SetWindowSize";
  external setTitle: (t, string) => unit = "resdl_SDL_SetWindowTitle";
  external setMinimumSize: (t, int, int) => unit =
    "resdl_SDL_SetWindowMinimumSize";

  external _enableHitTest: t => unit = "resdl_SDL_EnableHitTest";
  external _disableHitTest: t => unit = "resdl_SDL_EnableHitTest";

  let _idToHitTest: Hashtbl.t(int, hitTestCallback) = Hashtbl.create(16);

  let setHitTest = (win: t, cb: option(hitTestCallback)) => {
    switch (cb) {
    | None => _disableHitTest(win)
    | Some(v) =>
      _enableHitTest(win);
      Hashtbl.add(_idToHitTest, getId(win), v);
    };
  };

  let _hitTest = (win: t, x: int, y: int) => {
    let id = getId(win);
    switch (Hashtbl.find_opt(_idToHitTest, id)) {
    | Some(v) => v(win, x, y)
    | None => Normal
    };
  };

  Callback.register("__sdl2_caml_hittest__", _hitTest);

  external hide: t => unit = "resdl_SDL_HideWindow";
  external raise: t => unit = "resdl_SDL_RaiseWindow";
  external show: t => unit = "resdl_SDL_ShowWindow";

  external minimize: t => unit = "resdl_SDL_MinimizeWindow";
  external restore: t => unit = "resdl_SDL_RestoreWindow";
  external maximize: t => unit = "resdl_SDL_MaximizeWindow";

  external isMaximized: t => bool = "resdl_SDL_IsWindowMaximized";
  external isFullscreen: t => bool = "resdl_SDL_IsWindowFullscreen";

  external getDisplay: t => Display.t = "resdl_SDL_GetWindowDisplayIndex";

  // Windows-Only: Set DPI Aware process flag
  // Other platforms: no-op
  external setWin32ProcessDPIAware: t => unit =
    "resdl_SDL_SetWin32ProcessDPIAware";

  // WINDOWS-ONLY: Get the monitor scale factor for the window
  // Other platforms: Always returns 1.0
  external getWin32ScaleFactor: t => float = "resdl_SDL_GetWin32ScaleFactor";

  type nativeWindow;
  external getNativeWindow: t => nativeWindow = "resdl_SDL_GetNativeWindow";

  // MacOS-Only
  external setMacTitlebarTransparent: t => unit =
    "resdl_SDL_SetMacTitlebarTransparent";
  external setMacTitlebarHidden: t => unit = "resdl_SDL_SetMacTitlebarHidden";
  external setMacBackgroundColor: (t, float, float, float, float) => unit =
    "resdl_SDL_SetMacBackgroundColor";
  external getMacTitlebarHeight: t => float = "resdl_SDL_GetMacTitlebarHeight";
};

module Gl = {
  type context;

  type glString =
    | Vendor
    | Renderer
    | Version
    | ShadingLanguageVersion;
  type glInt =
    | FramebufferBinding;

  external setup: Window.t => context = "resdl_SDL_GL_Setup";
  external makeCurrent: (Window.t, context) => unit =
    "resdl_SDL_GL_MakeCurrent";
  external swapWindow: Window.t => unit = "resdl_SDL_GL_SwapWindow";
  external getDrawableSize: Window.t => Size.t =
    "resdl_SDL_GL_GetDrawableSize";
  external setSwapInterval: int => unit = "resdl_SDL_GL_SetSwapInterval";

  external getString: glString => string = "resdl_SDL_GL_GetString";
  external getFramebufferBinding: unit => int =
    "resdl_SDL_GL_GetFramebufferBinding";
};

external delay: int => unit = "resdl_SDL_Delay";
external init: unit => int = "resdl_SDL_Init";
external main: (int, array(string), unit => unit) => unit = "resdl_SDL_main";
let main = cb => main(Array.length(Sys.argv), Sys.argv, cb);

module TextInput = {
  [@noalloc] external start: unit => unit = "resdl_SDL_StartTextInput";
  [@noalloc] external stop: unit => unit = "resdl_SDL_StopTextInput";
  [@noalloc]
  external setInputRect: (int, int, int, int) => unit =
    "resdl_SDL_SetTextInputRect";
  [@noalloc] external isActive: unit => bool = "resdl_SDL_IsTextInputActive";
};

module MouseButton = {
  type t =
    | Left
    | Middle
    | Right
    | X1
    | X2;

  let show = v =>
    switch (v) {
    | Left => "Left"
    | Middle => "Middle"
    | Right => "Right"
    | X1 => "X1"
    | X2 => "X2"
    };
};

module Mouse = {
  external capture: bool => int = "resdl_SDL_CaptureMouse";
};

module Scancode = {
  type t = int;

  external getName: t => string = "resdl_SDL_GetScancodeName";

  external ofName: string => t = "resdl_SDL_GetScancodeFromName";

  [@noalloc] external ofInt: int => t = "resdl_PassThrough";
  [@noalloc] external toInt: t => int = "resdl_PassThrough";

  // Incrementally add these as needed from:
  // https://wiki.libsdl.org/SDLScancodeLookup
  let unknown = 0;
};

module Keycode = {
  type t = int;
  // as some keycodes are bigger than 2^30 but still less than 2^31
  // so by using the negative numbers we can fit them in a normal int
  let int = Int32.to_int;

  external getName: t => string = "resdl_SDL_GetKeyName";

  external ofName: string => t = "resdl_SDL_GetKeyFromName";

  [@noalloc]
  external ofScancode: Scancode.t => t = "resdl_SDL_GetKeyFromScancode";
  [@noalloc]
  external toScancode: t => Scancode.t = "resdl_SDL_GetScancodeFromKey";

  // Incrementally add these as needed from:
  // https://wiki.libsdl.org/SDLKeycodeLookup
  let unknown = int(0l);
  let backspace = int(8l);
  let return = int(13l);

  let escape = int(27l);

  let space = int(32l);

  let left_paren = int(40l);
  let right_paren = int(41l);

  let asterisk = int(42l);
  let plus = int(43l);
  let minus = int(45l);
  let period = int(46l);
  let slash = int(47l);
  let caret = int(94l);

  let equals = int(61l);

  let digit0 = int(48l);
  let digit1 = int(49l);
  let digit2 = int(50l);
  let digit3 = int(51l);
  let digit4 = int(52l);
  let digit5 = int(53l);
  let digit6 = int(54l);
  let digit7 = int(55l);
  let digit8 = int(56l);
  let digit9 = int(57l);

  let pad_divide = int(1073741908l);
  let pad_multiply = int(1073741909l);
  let pad_minus = int(1073741910l);
  let pad_plus = int(1073741911l);
  let pad_period = int(1073741923l);

  let pad_equals = int(1073741927l);

  let p_digit1 = int(1073741913l);
  let p_digit2 = int(1073741914l);
  let p_digit3 = int(1073741915l);
  let p_digit4 = int(1073741916l);
  let p_digit5 = int(1073741917l);
  let p_digit6 = int(1073741918l);
  let p_digit7 = int(1073741919l);
  let p_digit8 = int(1073741920l);
  let p_digit9 = int(1073741921l);
  let p_digit0 = int(1073741922l);

  let a = int(97l);
  let b = int(98l);
  let c = int(99l);
  let d = int(100l);
  let e = int(101l);
  let f = int(102l);
  let g = int(103l);
  let h = int(104l);
  let i = int(105l);
  let j = int(106l);
  let k = int(107l);
  let l = int(108l);
  let m = int(109l);
  let n = int(110l);
  let o = int(111l);
  let p = int(112l);
  let q = int(113l);
  let r = int(114l);
  let s = int(115l);
  let t = int(116l);
  let u = int(117l);
  let v = int(118l);
  let w = int(119l);
  let x = int(120l);
  let y = int(121l);
  let z = int(122l);

  let delete = int(127l);

  let right = int(1073741903l);
  let left = int(1073741904l);
};

module WheelType = {
  type t =
    | Last
    | Undefined
    | Touchscreen
    | Touchpad
    | Wheel
    | WheelPrecise
    | OtherNonKinetic
    | OtherKinetic;
};

module Keymod = {
  type t = int;

  let none = 0;

  [@noalloc] external isLeftShiftDown: t => bool = "resdl_SDL_ModLeftShift";
  [@noalloc] external isRightShiftDown: t => bool = "resdl_SDL_ModRightShift";

  let isShiftDown = v => isLeftShiftDown(v) || isRightShiftDown(v);

  [@noalloc]
  external isLeftControlDown: t => bool = "resdl_SDL_ModLeftControl";
  [@noalloc]
  external isRightControlDown: t => bool = "resdl_SDL_ModRightControl";

  let isControlDown = v => isLeftControlDown(v) || isRightControlDown(v);

  [@noalloc] external isLeftAltDown: t => bool = "resdl_SDL_ModLeftAlt";
  [@noalloc] external isRightAltDown: t => bool = "resdl_SDL_ModRightAlt";

  let isAltDown = v => isLeftAltDown(v) || isRightAltDown(v);

  [@noalloc] external isLeftGuiDown: t => bool = "resdl_SDL_ModLeftGui";
  [@noalloc] external isRightGuiDown: t => bool = "resdl_SDL_ModRightGui";

  let isGuiDown = v => isLeftGuiDown(v) || isRightGuiDown(v);

  [@noalloc] external isNumLockDown: t => bool = "resdl_SDL_ModNumLockDown";
  [@noalloc] external isCapsLockDown: t => bool = "resdl_SDL_ModCapsLockDown";

  [@noalloc] external isAltGrKeyDown: t => bool = "resdl_SDL_ModAltGrDown";

  let show = (v: t) => {
    let int_of_bool = b => b ? 1 : 0;

    Printf.sprintf(
      "Keymods - LSHIFT: %d RSHIFT: %d LCTRL: %d RCTRL: %d LALT: %d RALT: %d LGUI: %d RGUI: %d NUM: %d CAPS: %d ALTGR: %d",
      int_of_bool(isLeftShiftDown(v)),
      int_of_bool(isRightShiftDown(v)),
      int_of_bool(isLeftControlDown(v)),
      int_of_bool(isRightControlDown(v)),
      int_of_bool(isLeftAltDown(v)),
      int_of_bool(isRightAltDown(v)),
      int_of_bool(isLeftGuiDown(v)),
      int_of_bool(isRightGuiDown(v)),
      int_of_bool(isNumLockDown(v)),
      int_of_bool(isCapsLockDown(v)),
      int_of_bool(isAltGrKeyDown(v)),
    );
  };

  [@noalloc] external getState: unit => t = "resdl_SDL_GetModState";
  [@noalloc] external setState: t => unit = "resdl_SDL_SetModState";
};

module Event = {
  type mouseMotion = {
    windowID: int,
    x: int,
    y: int,
  };

  type mouseWheel = {
    windowID: int,
    deltaX: int,
    deltaY: int,
    isFlipped: bool,
  };

  type mousePan = {
    windowID: int,
    deltaX: int,
    deltaY: int,
    containsX: bool,
    containsY: bool,
    isFling: bool,
    isInterrupt: bool,
    source: WheelType.t,
    timestamp: int,
  };

  type mouseButtonEvent = {
    windowID: int,
    button: MouseButton.t,
    clicks: int,
    x: int,
    y: int,
  };

  type keyboardEvent = {
    windowID: int,
    repeat: bool,
    keymod: Keymod.t,
    scancode: Scancode.t,
    keycode: Keycode.t,
  };

  type textInputEvent = {
    windowID: int,
    text: string,
  };

  type textEditingEvent = {
    windowID: int,
    text: string,
    start: int,
    length: int,
  };

  type windowEvent = {windowID: int};

  type windowMoveEvent = {
    windowID: int,
    x: int,
    y: int,
  };

  type windowSizeEvent = {
    windowID: int,
    width: int,
    height: int,
  };

  /* Drop events
      dropNotificationEvent occurs when SDL is notifying when a drop
      is about to/has taken place through DROPBEGIN & DROPCOMPLETE

      dropEvent occurs when a file is actually being dropped

      The main difference is that dropEvent includes a file/text path, whereas
      dropNotificationEvent does not.
     */
  type dropNotificationEvent = {
    windowID: int,
    timestamp: int,
    x: int,
    y: int,
  };
  type dropEvent = {
    windowID: int,
    file: string,
    timestamp: int,
    x: int,
    y: int,
  };

  type t =
    | Quit
    | MouseMotion(mouseMotion) // 0
    | MouseWheel(mouseWheel) // 1
    | MouseButtonDown(mouseButtonEvent) // 2
    | MouseButtonUp(mouseButtonEvent) // 3
    | KeyDown(keyboardEvent) // 4
    | KeyUp(keyboardEvent) // 5
    | TextInput(textInputEvent) // 6
    | TextEditing(textEditingEvent) // 7
    | WindowShown(windowEvent) // 8
    | WindowHidden(windowEvent) // 9
    | WindowExposed(windowEvent) // 10
    | WindowMoved(windowMoveEvent) // 11
    | WindowResized(windowSizeEvent) // 12
    | WindowSizeChanged(windowSizeEvent) // 13
    | WindowMinimized(windowEvent) // 14
    | WindowMaximized(windowEvent) // 15
    | WindowRestored(windowEvent) // 16
    | WindowEnter(windowEvent) // 17
    | WindowLeave(windowEvent) // 18
    | WindowFocusGained(windowEvent) // 19
    | WindowFocusLost(windowEvent) // 20
    | WindowClosed(windowEvent) // 21
    | WindowTakeFocus(windowEvent) // 22
    | WindowHitTest(windowEvent) // 23
    | MousePan(mousePan) // 24
    | DropText(dropEvent) // 25
    | DropFile(dropEvent) // 26
    | DropBegin(dropNotificationEvent) // 27
    | DropComplete(dropNotificationEvent) // 28
    | WindowFullscreen(windowEvent) // 29
    // An event that hasn't been implemented yet
    | Unknown
    | KeymapChanged;

  let show = (v: t) => {
    switch (v) {
    | Quit => "Quit"
    | MouseMotion({windowID, x, y}) =>
      Printf.sprintf("MouseMotion windowId: %d x: %d y: %d", windowID, x, y)
    | MouseWheel({windowID, deltaX, deltaY, isFlipped}) =>
      Printf.sprintf(
        "MouseWheel windowId: %d x: %d y: %d isFlipped: %d",
        windowID,
        deltaX,
        deltaY,
        isFlipped ? 1 : 0,
      )
    | MousePan({
        windowID,
        deltaX,
        deltaY,
        containsX,
        containsY,
        isFling,
        isInterrupt,
        _,
      }) =>
      Printf.sprintf(
        "Pan event: %d %d %d %d %d %d %d",
        windowID,
        deltaX,
        deltaY,
        if (containsX) {1} else {0},
        if (containsY) {1} else {0},
        if (isFling) {1} else {0},
        if (isInterrupt) {1} else {0},
      )
    | MouseButtonUp({windowID, button, _}) =>
      Printf.sprintf(
        "MouseButtonUp windowId: %d button: %s",
        windowID,
        MouseButton.show(button),
      )
    | MouseButtonDown({windowID, button, _}) =>
      Printf.sprintf(
        "MouseButtonDown windowId: %d button: %s",
        windowID,
        MouseButton.show(button),
      )
    | KeyDown({repeat, keymod, scancode, keycode, _}) =>
      Printf.sprintf(
        "KeyDown repeat %d:\n -- %s\n -- Scancode: %s\n -- Keycode: %s\n",
        repeat ? 1 : 0,
        Keymod.show(keymod),
        Scancode.getName(scancode),
        Keycode.getName(keycode),
      )
    | KeyUp({repeat, keymod, scancode, keycode, _}) =>
      Printf.sprintf(
        "KeyUp repeat %d:\n -- %s\n -- Scancode: %s\n -- Keycode: %s\n",
        repeat ? 1 : 0,
        Keymod.show(keymod),
        Scancode.getName(scancode),
        Keycode.getName(keycode),
      )
    | TextEditing({text, start, length, _}) =>
      Printf.sprintf(
        "TextEditing:\n -- start: %d length: %d text: %s\n",
        start,
        length,
        text,
      )
    | TextInput({text, _}) =>
      Printf.sprintf("TextInput:\n -- text: %s\n", text)
    | WindowShown({windowID}) =>
      Printf.sprintf("WindowShown: %d\n", windowID)
    | WindowHidden({windowID}) =>
      Printf.sprintf("WindowHidden: %d\n", windowID)
    | WindowExposed({windowID}) =>
      Printf.sprintf("WindowExposed: %d\n", windowID)
    | WindowMoved({windowID, x, y}) =>
      Printf.sprintf(
        "WindowMoved - windowID: %d x: %d y: %d\n",
        windowID,
        x,
        y,
      )
    | WindowResized({windowID, width, height}) =>
      Printf.sprintf(
        "WindowResized - windowID: %d x: %d y: %d\n",
        windowID,
        width,
        height,
      )
    | WindowSizeChanged({windowID, width, height}) =>
      Printf.sprintf(
        "WindowSizeChanged - windowID: %d x: %d y: %d\n",
        windowID,
        width,
        height,
      )
    | WindowMinimized({windowID}) =>
      Printf.sprintf("WindowMinimized: %d\n", windowID)
    | WindowMaximized({windowID}) =>
      Printf.sprintf("WindowMaximized: %d\n", windowID)
    | WindowFullscreen({windowID}) =>
      Printf.sprintf("WindowFullscreen: %d\n", windowID)
    | WindowRestored({windowID}) =>
      Printf.sprintf("WindowRestored: %d\n", windowID)
    | WindowEnter({windowID}) =>
      Printf.sprintf("WindowEnter: %d\n", windowID)
    | WindowLeave({windowID}) =>
      Printf.sprintf("WindowLeave: %d\n", windowID)
    | WindowFocusGained({windowID}) =>
      Printf.sprintf("WindowFocusGained: %d\n", windowID)
    | WindowFocusLost({windowID}) =>
      Printf.sprintf("WindowFocusLost: %d\n", windowID)
    | WindowClosed({windowID}) =>
      Printf.sprintf("WindowClosed: %d\n", windowID)
    | WindowTakeFocus({windowID}) =>
      Printf.sprintf("WindowTakeFocus: %d\n", windowID)
    | WindowHitTest({windowID}) =>
      Printf.sprintf("WindowHitTest: %d\n", windowID)
    | DropText({windowID, file, x, y, _}) =>
      Printf.sprintf(
        "DropText - windowID: %d file: %s x: %d y: %d\n",
        windowID,
        file,
        x,
        y,
      )
    | DropFile({windowID, file, x, y, _}) =>
      Printf.sprintf(
        "DropFile - windowID: %d file: %s x: %d y: %d\n",
        windowID,
        file,
        x,
        y,
      )
    | DropBegin({windowID, x, y, _}) =>
      Printf.sprintf("DropBegin - windowID: %d x: %d y: %d\n", windowID, x, y)
    | DropComplete({windowID, x, y, _}) =>
      Printf.sprintf(
        "DropComplete - windowID: %d x: %d y: %d\n",
        windowID,
        x,
        y,
      )
    | KeymapChanged => "KeymapChanged"
    | Unknown => "Unknown"
    };
  };

  external poll: unit => option(t) = "resdl_SDL_PollEvent";
  external push: unit => unit = "resdl_SDL_PushEvent";
  external wait: unit => result(t, string) = "resdl_SDL_WaitEvent";
  external waitTimeout: int => option(t) = "resdl_SDL_WaitTimeoutEvent";
};

module Cursor = {
  type systemCursor =
    | Arrow
    | IBeam
    | Wait
    | Crosshair
    | WaitArrow
    | SizeNWSE
    | SizeNESW
    | SizeWE
    | SizeNS
    | SizeAll
    | No
    | Hand;

  type t;

  external createSystem: systemCursor => t = "resdl_SDL_CreateSystemCursor";
  external set: t => unit = "resdl_SDL_SetCursor";
};

module MessageBox = {
  type flags =
    | Error
    | Warning
    | Information;

  external showSimple: (flags, string, string, option(Window.t)) => unit =
    "resdl_SDL_ShowSimpleMessageBox";
};

module Timekeeping = {
  external getTicks: unit => int = "resdl_SDL_GetTicks";
};

module Version = {
  type t = {
    major: int,
    minor: int,
    patch: int,
  };

  external getCompiled: unit => t = "resdl_SDL_GetCompiledVersion";
  external getLinked: unit => t = "resdl_SDL_GetLinkedVersion";

  let toString: t => string =
    ({major, minor, patch}) =>
      Printf.sprintf("Major: %d Minor: %d Patch: %d", major, minor, patch);
};

type renderFunction = unit => bool;
external _javaScriptRenderLoop: renderFunction => unit =
  "resdl__javascript__renderloop";
external _javaScriptRenderLoop: renderFunction => unit =
  "resdl__javascript__renderloop";

let _nativeLoop = renderFn => {
  while (!renderFn()) {
    ();
      /*Thread.yield();*/
  };
  ();
};

let renderLoop = (renderFunction: renderFunction) => {
  switch (Sys.backend_type) {
  | Native => _nativeLoop(renderFunction)
  | Bytecode => _nativeLoop(renderFunction)
  | _ => _javaScriptRenderLoop(renderFunction)
  };
};
