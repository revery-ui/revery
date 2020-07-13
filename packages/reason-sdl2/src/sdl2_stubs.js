//Provides: resdl_SDL_Init
function resdl_SDL_Init() {
    joo_global_object._time = {
        start: Date.now(),
        offset: 0,
    };

    joo_global_object._events = [];
    joo_global_object._popEvent = function () {
        if (joo_global_object._events.length > 0) {
            return joo_global_object._events.shift();
        } else {
            return null;
        }
    };

    joo_global_object._pushEvent = function (evt) {
        joo_global_object._events.push(evt);
    };

    joo_global_object.window.addEventListener("resize", function () {
        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            wins[i]._notifyResize();
        }
    });

    joo_global_object._mouseState = {};
    joo_global_object.window.addEventListener("mousemove", function (e) {
        var activeWindow = joo_global_object._activeWindow;
        joo_global_object._mouseState.x = e.pageX - activeWindow.x;
        joo_global_object._mouseState.y = e.pageY - activeWindow.y;

        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            var win = wins[i];
            win._notifyMouseMove(e.pageX - win.x, e.pageY - win.y);
        }
    });

    joo_global_object.window.addEventListener("wheel", function (e) {
        // TODO: account for deltaMode
        var deltaX = e.deltaX;
        var deltaY = e.deltaY;

        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            wins[i]._notifyScroll(deltaX, deltaY);
        }
    });

    joo_global_object.window.addEventListener("keypress", function (keyEvent) {
        if (keyEvent.key && keyEvent.key.length === 1) {
            var codepoint = keyEvent.key.codePointAt(0);
            var wins = joo_global_object._activeWindows;
            for (var i = 0; i < wins.length; i++) {
                wins[i]._notifyChar(codepoint);
            }
        }
    });

    joo_global_object.window.addEventListener("mousedown", function (mouseEvent) {
        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            wins[i]._notifyMouseButton(mouseEvent, 2);
        }
    });

    joo_global_object.window.addEventListener("mouseup", function (mouseEvent) {
        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            wins[i]._notifyMouseButton(mouseEvent, 3);
        }
    });

    joo_global_object.window.addEventListener("keydown", function (keyEvent) {
        if (keyEvent.key) {
            var wins = joo_global_object._activeWindows;
            for (var i = 0; i < wins.length; i++) {
                var pressMode = keyEvent.repeat ? 2 : 0;
                wins[i]._notifyKey(keyEvent, pressMode);
            }
        }
    });

    joo_global_object.window.addEventListener("keyup", function (keyEvent) {
        if (keyEvent.key) {
            var wins = joo_global_object._activeWindows;
            for (var i = 0; i < wins.length; i++) {
                wins[i]._notifyKey(keyEvent, 1);
            }
        }
    });
};

// Provides: caml_glfwGetCursorPos
// Requires: caml_js_to_array
function caml_glfwGetCursorPos(w) {
    // TODO: Window parameter is currently ignored, but
    // we should calculate the mouse position relative to it.

    return caml_js_to_array([joo_global_object._mouseState.x, joo_global_object._mouseState.y]);
}

// Provides: resdl_SDL_CreateSystemCursor
function resdl_SDL_CreateSystemCursor(shape) {
  switch (shape) {
  case 0: return "default";
  case 1: return "text";
  case 3: return "crosshair";
  case 4: return "wait";
  case 5: return "nwse-resize";
  case 6: return "nesw-resize";
  case 7: return "we-resize";
  case 8: return "ns-resize";
  case 9: return "resize";
  case 10: return "not-allowed";
  case 11: return "pointer";
  default:
    joo_global_object.console.warn("Unsupported cursor shape.");
    return "default";
  }
}

// Provides: resdl_SDL_EnableScreenSaver
function resdl_SDL_EnableScreenSaver() {
  // no op
}

// Provides: resdl_SDL_DisableScreenSaver
function resdl_SDL_DisableScreenSaver() {
  // no op
}

// Provides: resdl_SDL_IsScreenSaverEnabled
function resdl_SDL_IsScreenSaverEnabled() {
  return false;
}

// Provides: resdl_SDL_GL_SetSwapInterval
function resdl_SDL_GL_SetSwapInterval() {
  // no op
}

// Provides: resdl_SDL_PushEvent
function resdl_SDL_PushEvent() {
  // no op
}

// Provides: resdl_SDL_GL_MakeCurrent
function resdl_SDL_GL_MakeCurrent() {
  // no op
}

// Provides: resdl_SDL_SetWin32ProcessDPIAware
function resdl_SDL_SetWin32ProcessDPIAware() {
  // no op
}

// Provides: resdl_SDL_GetWin32ScaleFactor
function resdl_SDL_GetWin32ScaleFactor() {
  return 1;
}

// Provides: resdl_SDL_GetWindowDisplayIndex
function resdl_SDL_GetWindowDisplayIndex() {
  return 0;
}

// Provides: resdl_SDL_GetDisplayDPI
function resdl_SDL_GetDisplayDPI() {
  return 96;
}

// Provides: resdl_SDL_GetDesktopDisplayMode
function resdl_SDL_GetDesktopDisplayMode() {
  return [0, 100, 200, 0];
}

// Provides: resdl_SDL_WindowCenter
function resdl_SDL_WindowCenter() {
  //no-op
}

// Provides: caml_glfwDestroyCursor
function caml_glfwDestroyCursor(cursor) {
  // no op
}

// Provides: resdl_SDL_SetCursor
function resdl_SDL_SetCursor(cursor) {
  joo_global_object._activeWindow.canvas.style.cursor = cursor;
}

// Provides: resdl_SDL_SetWindowMinimumSize
function resdl_SDL_SetWindowMinimumSize(win, minWidth, minHeight) {
    // no-op
}

// Provides: caml_glfwGetTime_byte
function caml_glfwGetTime_byte() {
    return (joo_global_object._time.offset + (Date.now() - joo_global_object._time.start)) / 1000;
}

// Provides: caml_glfwSetTime_byte
function caml_glfwSetTime_byte(t) {
    joo_global_object._time.offset = t * 1000;
    joo_global_object._time.start = Date.now();
}

// Provides: caml_glfwGetPrimaryMonitor
function caml_glfwGetPrimaryMonitor() {
    // No-op
}

// Provides: caml_glfwGetVideoMode
function caml_glfwGetVideoMode() {
    var win = joo_global_object.window;
    return [0, win.innerWidth, win.innerHeight];
};

// Provides: caml_glfwGetMonitorPos
function caml_glfwGetMonitorPos() {
    return [0, 0, 0];
};

// Provides: caml_glfwGetMonitorPhysicalSize
function caml_glfwGetMonitorPhysicalSize() {
  var win = joo_global_object.window;
  var dpi = 96;
  var widthMM = (win.innerWidth * 25.4) / dpi;
  var heightMM = (win.innerHeight * 25.4) / dpi;
  return [0, widthMM, heightMM];
};

// Provides: resdl_SDL_GetWindowSize
function resdl_SDL_GetWindowSize(w) {
    var pixelRatio = joo_global_object.window.devicePixelRatio;
    var width = w.canvas.width / pixelRatio;
    var height = w.canvas.height / pixelRatio;
    return [0, width, height];
}

// Provides: resdl_SDL_GetWindowId
function resdl_SDL_GetWindowId(w) {
    return 0;
}

// Provides: resdl_SDL_GL_GetDrawableSize
function resdl_SDL_GL_GetDrawableSize(w) {
    var pixelRatio = joo_global_object.window.devicePixelRatio;
    var width = w.canvas.width;
    var height = w.canvas.height;
    return [0, width, height];
}

// Provides: caml_glfwDefaultWindowHints
function caml_glfwDefaultWindowHints(w) {
    joo_global_object.console.warn("glfwDefaultWindowHints not implemented in WebGL");
}

// Provides: caml_glfwShowWindow
function caml_glfwShowWindow(w) {
    joo_global_object.console.warn("glfwShowWindow not implemented in WebGL");
}

// Provides: caml_glfwHideWindow
function caml_glfwHideWindow(w) {
    joo_global_object.console.warn("glfwHideWindow not implemented in WebGL");
}

// Provides: caml_test_callback_success
function caml_test_callback_success(s, f) {
    s(999);
}

// Provides: resdl__javascript__renderloop
function resdl__javascript__renderloop(loopFunc) {
    function renderLoop(t) {
        var ret = loopFunc(t);

        if (!ret) {
            joo_global_object.window.requestAnimationFrame(renderLoop);
        }
    }

    joo_global_object.window.requestAnimationFrame(renderLoop);
}

//Provides: resdl_SDL_ShowWindow
function resdl_SDL_ShowWindow(w) {
    // no-op
}

//Provides: resdl_SDL_CreateWindow
function resdl_SDL_CreateWindow(width, height, title) {
    var canvas = document.createElement("canvas");
    canvas.style.position = "absolute";
    canvas.style.top = "0px";
    canvas.style.bottom = "0px";
    canvas.style.width = width.toString() + "px";
    canvas.style.height = height.toString() + "px";
    var pixelRatio = joo_global_object.window.devicePixelRatio;
    canvas.width = width * pixelRatio;
    canvas.height = height * pixelRatio;

    joo_global_object._activeWindows = joo_global_object._activeWindows || [];

    document.body.appendChild(canvas);
    var w = {
        id: 0,
        canvas: canvas,
        title: title,
        isMaximized: false,
        onSetFramebufferSize: null,
        onSetWindowSize: null,
        onKey: null,
        onChar: null,
        x: 0,
        y: 0,
    };

    var notifyChar = function (codepoint) {
        if (w.onChar) {
            w.onChar(w, codepoint);
        }
    };

    var notifyKey = function (keyEvent, pressMode) {
        if (w.onKey) {

            if (keyEvent.key && keyEvent.key.length == 1) {
                var code = keyEvent.key.toUpperCase().charCodeAt(0);
                var modifier = 0;
                modifier = keyEvent.shiftKey ? modifier + 1 : modifier;
                modifier = keyEvent.ctrlKey ? modifier + 2 : modifier;
                modifier = keyEvent.altKey ? modifier + 4 : modifier;
                modifier = keyEvent.metaKey ? modifier + 8 : modifier

                w.onKey(w, code, keyEvent.location, pressMode, modifier);
            };
        }
    };

    var notifyMouseButton = function(mouseEvent, pressMode) {
        joo_global_object._pushEvent(
        [pressMode, 
            [0, 
                w.id, 
                0, 
                0, 
                joo_global_object._mouseState.x, 
                joo_global_object._mouseState.y]]);
    };

    var notifyMouseMove = function(x, y) {
        joo_global_object._pushEvent(
        [0, [0, w.id, x, y]]
        );
    };

    var notifyResize = function () {
        if (w.isMaximized) {
            var pixelRatio = joo_global_object.window.devicePixelRatio;

            canvas.width = canvas.offsetWidth * pixelRatio;
            canvas.height = canvas.offsetHeight * pixelRatio;

            joo_global_object._pushEvent([12, [0, w.id, canvas.offsetWidth, canvas.offsetHeight]]);
        }
    };

    var notifyScroll = function (x, y) {
        joo_global_object._pushEvent([1, [0, w.id, x, y, false]]);
    };

    w._notifyResize = notifyResize;
    w._notifyChar = notifyChar;
    w._notifyScroll = notifyScroll;
    w._notifyKey = notifyKey;
    w._notifyMouseButton = notifyMouseButton;
    w._notifyMouseMove = notifyMouseMove;

    joo_global_object._activeWindows.push(w);
    return w;
};

// Provides: resdl_SDL_SetWindowPosition
function resdl_SDL_SetWindowPosition(w, x, y) {
    var canvas = w.canvas;
    canvas.style.transform = "translate(" + x.toString() + "px, " + y.toString() + "px)";
    w.x = x;
    w.y = y;
}

// Provides: resdl_SDL_SetWindowSize
function resdl_SDL_SetWindowSize(w, width, height) {
    var canvas = w.canvas;
    canvas.style.width = width.toString() + "px";
    canvas.style.height = height.toString() + "px";
    var pixelRatio = joo_global_object.window.devicePixelRatio;
    canvas.width = width * pixelRatio;
    canvas.height = height * pixelRatio;
}

// Provides: resdl_SDL_SetWindowTitle
function resdl_SDL_SetWindowTitle(w, title) {
    var t = title.toString();
    w.title = t;
    document.title = t;
}

// Provides: resdl_SDL_CreateRGBSurfaceFromImage
function resdl_SDL_CreateRGBSurfaceFromImage(path) {
    return [0, path];
};

// Provides: resdl_SDL_SetWindowIcon
function resdl_SDL_SetWindowIcon(w, surface) {
    var p = surface.toString();
    var link = document.querySelector("link[rel*='icon']") || document.createElement('link');
    link.type = 'image/x-icon';
    link.rel = 'shortcut icon';
    link.href = p;
    document.getElementsByTagName('head')[0].appendChild(link);
}

// Provides: resdl_SDL_SetTextInputRect
function resdl_SDL_SetTextInputRect(x, y, w, h) {
    // TODDO: No-op
}

// Provides: resdl_SDL_StartTextInput
function resdl_SDL_StartTextInput() {
    // TODDO: No-op
}

// Provides: resdl_SDL_MaximizeWindow
function resdl_SDL_MaximizeWindow(w) {
    if (w && !w.isMaximized) {
        var pixelRatio = joo_global_object.window.devicePixelRatio;
        var canvas = w.canvas;
        canvas.style.width = "100%";
        canvas.style.height = "100%";
        canvas.width = canvas.offsetWidth * pixelRatio;
        canvas.height = canvas.offsetHeight * pixelRatio;
        w.isMaximized = true;
        w._notifyResize();
    }
}

// Provides: resdl_SDL_GL_Setup
function resdl_SDL_GL_Setup(win) {
    var gl = win.canvas.getContext('webgl');
    joo_global_object.window.__glfw__gl__ = gl;
    joo_global_object.window._activeWindow = win;

    joo_global_object.variantToTextureType = {
        '0': gl.TEXTURE_2D,
    };

    joo_global_object.variantToEnableOption = {
        '0': gl.DEPTH_TEST,
        '1': gl.BLEND,
        '2': gl.SCISSOR_TEST,
    };

    joo_global_object.variantToBlendFunc = {
        '0': gl.ZERO,
        '1': gl.ONE,
        '2': gl.SRC_ALPHA,
        '3': gl.ONE_MINUS_SRC_ALPHA,
    };

    joo_global_object.variantToTextureParameter = {
        '0': gl.TEXTURE_WRAP_S,
        '1': gl.TEXTURE_WRAP_T,
        '2': gl.TEXTURE_MIN_FILTER,
        '3': gl.TEXTURE_MAG_FILTER,
    };

    joo_global_object.variantToPixelAlignmentParameter = {
        '0': gl.PACK_ALIGNMENT,
        '1': gl.UNPACK_ALIGNMENT,
    };

    joo_global_object.variantToTextureParameterValue = {
        '0': gl.REPEAT,
        '1': gl.LINEAR,
        '2': gl.CLAMP_TO_EDGE,
    }

    joo_global_object.variantToFormat = {
        '0': gl.ALPHA,
        '1': gl.RGB,
        '2': gl.RGBA,
    }

    joo_global_object.formatToNumChannels = {
        '6406': 1, // ALPHA
        '6407': 3, // RGB
        '6408': 4, // RGBA
    }

    joo_global_object.variantToType = {
        '0': gl.FLOAT,
        '1': gl.UNSIGNED_BYTE,
        '2': gl.UNSIGNED_SHORT,
    }

    joo_global_object.variantToBufferType = {
        '0': gl.ARRAY_BUFFER,
        '1': gl.ELEMENT_ARRAY_BUFFER,
    }

    joo_global_object.variantToDrawMode = {
        '0': gl.TRIANGLES,
        '1': gl.TRIANGLE_STRIP
    }

    joo_global_object.gl = gl;
}

// Provides: resdl_SDL_PollEvent
function resdl_SDL_PollEvent() {
    var evt = joo_global_object._popEvent();
    if (evt != null) {
        return [0, evt];
    } else {
        return 0;
    }
    return 0;
}
// Provides: resdl_SDL_ShowSimpleMessageBox
function resdl_SDL_ShowSimpleMessageBox(flags, title, msg, win) {
    joo_global_object.alert(msg);
}

// Provides: resdl_SDL_GetNativeWindow
function resdl_SDL_GetNativeWindow(win) {
    // TODO: Return canvas?
    return null;
}

// Provides: resdl_SDL_WinAttachConsole
function resdl_SDL_WinAttachConsole() {
    // no-op
    return 1;
}

// Provides: resdl_SDL_WinAllocConsole
function resdl_SDL_WinAllocConsole() {
    // no-op
    return 1;
}

// Provides: resdl_SDL_GetPlatform
// Requires: caml_js_to_string
function resdl_SDL_GetPlatform() {
    return caml_js_to_string("Browser");
}

// Provides: resdl_SDL_GetVersion
// Requires: caml_js_to_string
function resdl_SDL_GetVersion() {
    return caml_js_to_string("Not implemented");
}

// Provides: resdl_SDL_SetMacBackgroundColor
function resdl_SDL_SetMacBackgroundColor() {
    // No-op
}

// Provides: resdl_SDL_SetMacTitlebarTransparent
function resdl_SDL_SetMacTitlebarTransparent() {
    // No-op
}

// Provides: resdl_SDL_EnableHitTest
function resdl_SDL_EnableHitTest() {
    // No-op
}

// Provides: resdl_SDL_DisableHitTest
function resdl_SDL_DisableHitTest() {
    // No-op
}

// Provides: resdl_SDL_SetWindowBordered
function resdl_SDL_SetWindowBordered() {
    // No-op
}

// Provides: resdl_SDL_SetWindowResizable
function resdl_SDL_SetWindowResizable() {
    // No-op
}

// Provides: resdl_SDL_GetModState
function resdl_SDL_GetModState() {
    return 0;
}

// Provides: resdl_SDL_WaitTimeoutEvent
// Requires: resdl_SDL_PollEvent
function resdl_SDL_WaitTimeoutEvent() {
    joo_global_object.console.warn("waitTimeout not supported in JSOO");
    return resdl_SDL_PollEvent();
}

// Provides: resdl_SDL_GL_SwapWindow
function resdl_SDL_GL_SwapWindow() {
    // no op
}
