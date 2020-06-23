#include <cstdint>
#include <cstdio>

#include <algorithm>
#include <array>

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

#include "stb_image.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#endif

#ifdef WIN32
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#endif

#include <glad/glad.h>

#define Val_none Val_int(0)
static value Val_some(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 0);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

static value Val_ok(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 0);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

static value Val_error(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 1);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

extern "C" {
CAMLprim value resdl_SDL_EnableScreenSaver() {
  CAMLparam0();
  SDL_EnableScreenSaver();
  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_DisableScreenSaver() {
  CAMLparam0();
  SDL_DisableScreenSaver();
  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_IsScreenSaverEnabled() {
  CAMLparam0();
  int result = SDL_IsScreenSaverEnabled() == SDL_TRUE;
  CAMLreturn(Val_int(result));
}

CAMLprim value resdl_SDL_SetMainReady() {
  SDL_SetMainReady();

  return Val_unit;
}
CAMLprim value resdl_SDL_DestroyWindow(value vWin) {
  SDL_Window *win = (SDL_Window *)vWin;
  SDL_DestroyWindow(win);
  return Val_unit;
}

SDL_HitTestResult resdl_hit_test(SDL_Window *win, const SDL_Point *area,
                                 void *data) {

  static const value *hitTestCallback = NULL;
  if (hitTestCallback == NULL) {
    hitTestCallback = caml_named_value("__sdl2_caml_hittest__");
  }
  value vWin = (value)win;
  value vRet = caml_callback3(*hitTestCallback, vWin, Val_int(area->x),
                              Val_int(area->y));
  SDL_HitTestResult result;
  switch (Int_val(vRet)) {
  case 0:
    result = SDL_HITTEST_NORMAL;
    break;
  case 1:
    result = SDL_HITTEST_DRAGGABLE;
    break;
  case 2:
    result = SDL_HITTEST_RESIZE_TOPLEFT;
    break;
  case 3:
    result = SDL_HITTEST_RESIZE_TOP;
    break;
  case 4:
    result = SDL_HITTEST_RESIZE_TOPRIGHT;
    break;
  case 5:
    result = SDL_HITTEST_RESIZE_RIGHT;
    break;
  case 6:
    result = SDL_HITTEST_RESIZE_BOTTOMRIGHT;
    break;
  case 7:
    result = SDL_HITTEST_RESIZE_BOTTOM;
    break;
  case 8:
    result = SDL_HITTEST_RESIZE_BOTTOMLEFT;
    break;
  case 9:
    result = SDL_HITTEST_RESIZE_LEFT;
    break;
  default:
    result = SDL_HITTEST_NORMAL;
    break;
  }

  return result;
};

CAMLprim value resdl_SDL_EnableHitTest(value vWin) {
  SDL_Window *win = (SDL_Window *)vWin;
  SDL_SetWindowHitTest(win, resdl_hit_test, NULL);
  return Val_unit;
}

CAMLprim value resdl_SDL_DisableHitTest(value vWin) {
  SDL_Window *win = (SDL_Window *)vWin;
  SDL_SetWindowHitTest(win, NULL, NULL);
  return Val_unit;
}

CAMLprim value resdl_SDL_Delay(value delay) {
  CAMLparam1(delay);

  int d = Int_val(delay);
  SDL_Delay(d);
  CAMLreturn(Val_unit);
}

#ifdef WIN32

typedef enum PROCESS_DPI_AWARENESS {
  PROCESS_DPI_UNAWARE = 0,
  PROCESS_SYSTEM_DPI_AWARE = 1,
  PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

HWND getHWNDFromSDLWindow(SDL_Window *win) {
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(win, &wmInfo);
  return wmInfo.info.win.window;
};

#endif

CAMLprim value resdl_SDL_GetPlatform() {
  CAMLparam0();
  CAMLlocal1(ret);

  const char *str = SDL_GetPlatform();
  ret = caml_copy_string(str);

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GetVersion() {
  CAMLparam0();
  CAMLlocal1(ret);

  const char *str;
#ifdef __APPLE__
  NSProcessInfo *pInfo = [NSProcessInfo processInfo];
  NSString *version = [pInfo operatingSystemVersionString];
  str = [version UTF8String];
#else
  str = "??.??.??";
#endif
  ret = caml_copy_string(str);

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GetNativeWindow(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(win, &wmInfo);

  void *pNativeWindow = NULL;
  switch (wmInfo.subsystem) {
#ifdef WIN32
  case SDL_SYSWM_WINDOWS:
    pNativeWindow = (void *)wmInfo.info.win.window;
    break;
#elif __APPLE__
  case SDL_SYSWM_COCOA:
    pNativeWindow = (void *)wmInfo.info.cocoa.window;
    break;
#else
  case SDL_SYSWM_X11:
    pNativeWindow = (void *)wmInfo.info.x11.window;
    break;
    // TODO: Do we need a compilation flag to enable wayland support?
    /*
    case SDL_SYSWM_WAYLAND:
      pNativeWindow = (void *)wmInfo.info.wl.surface;
      break;
    */
#endif
  default:
    break;
  }

  CAMLreturn((value)pNativeWindow);
};

#ifdef WIN32

// This method is calling after attach / alloc console
// to wire up the new stdin/stdout/stderr.
// See further details (thanks @dra27 for the help!)
// - https://github.com/ocaml/ocaml/issues/9252
void resdl_Win32AttachStdIO() {
  int fd_in = _open_osfhandle((intptr_t)GetStdHandle(STD_INPUT_HANDLE),
                              _O_RDONLY | _O_BINARY);
  int fd_out = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE),
                               _O_WRONLY | _O_BINARY);
  int fd_err = _open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE),
                               _O_WRONLY | _O_BINARY);

  if (fd_in) {
    dup2(fd_in, 0);
    close(fd_in);
    SetStdHandle(STD_INPUT_HANDLE, (HANDLE)_get_osfhandle(0));
  }

  if (fd_out) {
    dup2(fd_out, 1);
    close(fd_out);
    SetStdHandle(STD_OUTPUT_HANDLE, (HANDLE)_get_osfhandle(1));
  }

  if (fd_err) {
    dup2(fd_err, 2);
    close(fd_err);
    SetStdHandle(STD_ERROR_HANDLE, (HANDLE)_get_osfhandle(2));
  }
  *stdin = *(fdopen(0, "rb"));
  *stdout = *(fdopen(1, "wb"));
  *stderr = *(fdopen(2, "wb"));

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
}
#endif

CAMLprim value resdl_SDL_WinAttachConsole() {
  CAMLparam0();
  int ret = 0;
#ifdef WIN32
  // Only attach if we don't already have a stdout handle
  if (GetStdHandle(STD_OUTPUT_HANDLE) == NULL) {
    ret = AttachConsole(ATTACH_PARENT_PROCESS);
    if (ret) {
      resdl_Win32AttachStdIO();
    }
  } else {
    // There's already a stdout handle available,
    // so return success
    ret = 1;
  }
#endif
  CAMLreturn(Val_int(ret));
}

CAMLprim value resdl_SDL_WinAllocConsole() {
  CAMLparam0();
  int ret = 0;
#ifdef WIN32
  ret = AllocConsole();
  if (ret) {
    resdl_Win32AttachStdIO();
  }
#endif
  CAMLreturn(Val_int(ret));
}

CAMLprim value resdl_SDL_SetMacTitlebarTransparent(value vWin) {
  CAMLparam1(vWin);

#ifdef __APPLE__
  SDL_Window *win = (SDL_Window *)vWin;
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(win, &wmInfo);
  NSWindow *nWindow = wmInfo.info.cocoa.window;
  [nWindow
      setStyleMask:[nWindow styleMask] | NSWindowStyleMaskFullSizeContentView];
  [nWindow setTitlebarAppearsTransparent:YES];
  nWindow.titleVisibility = NSWindowTitleHidden;
#endif

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_SetMacBackgroundColor(value vWin, value r, value g,
                                               value b, value a) {
  CAMLparam5(vWin, r, g, b, a);

#ifdef __APPLE__
  SDL_Window *win = (SDL_Window *)vWin;
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(win, &wmInfo);
  double red = Double_val(r);
  double green = Double_val(g);
  double blue = Double_val(b);
  double alpha = Double_val(a);
  NSWindow *nWindow = wmInfo.info.cocoa.window;
  NSColor *rgb =
      [NSColor colorWithDeviceRed:red green:green blue:blue alpha:alpha];
  [nWindow setBackgroundColor:rgb];
#endif

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_SetWin32ProcessDPIAware(value vWin) {
  CAMLparam1(vWin);

#ifdef WIN32
  void *userDLL;
  BOOL(WINAPI * SetProcessDPIAware)(void); // Vista and later
  void *shcoreDLL;
  HRESULT(WINAPI * SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS dpiAwareness);

  userDLL = SDL_LoadObject("USER32.DLL");
  if (userDLL) {
    SetProcessDPIAware =
        (BOOL(WINAPI *)(void))SDL_LoadFunction(userDLL, "SetProcessDPIAware");
  }

  shcoreDLL = SDL_LoadObject("SHCORE.DLL");
  if (shcoreDLL) {
    SetProcessDpiAwareness =
        (HRESULT(WINAPI *)(PROCESS_DPI_AWARENESS))SDL_LoadFunction(
            shcoreDLL, "SetProcessDpiAwareness");
  }

  if (SetProcessDpiAwareness) {
    // Try Windows 8.1+ version
    HRESULT result = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
  } else if (SetProcessDPIAware) {
    // Try Vista - Windows 8 version.
    // This has a constant scale factor for all monitors.
    BOOL success = SetProcessDPIAware();
  }
#endif

  CAMLreturn(Val_unit);
};

CAMLprim value resdl_SDL_GetWin32ScaleFactor(value vWin) {
  CAMLparam1(vWin);

#ifdef WIN32
  SDL_Window *win = (SDL_Window *)vWin;
  HWND hwnd = getHWNDFromSDLWindow(win);
  HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);

  void *shcoreDLL;
  HRESULT(WINAPI * GetScaleFactorForMonitor)(HMONITOR hmon, int *pScale);

  shcoreDLL = SDL_LoadObject("SHCORE.DLL");
  if (shcoreDLL) {
    GetScaleFactorForMonitor =
        (HRESULT(WINAPI *)(HMONITOR, int *))SDL_LoadFunction(
            shcoreDLL, "GetScaleFactorForMonitor");
  }

  if (GetScaleFactorForMonitor) {
    int pScale;
    GetScaleFactorForMonitor(hmon, &pScale);
    CAMLreturn(caml_copy_double(pScale / 100.0));
  } else {
    CAMLreturn(caml_copy_double(1.0));
  }

#else
  CAMLreturn(caml_copy_double(1.0));
#endif
};

CAMLprim value resdl_SDL_GetDisplayDPI(value vDisplay) {
  CAMLparam1(vDisplay);
  CAMLlocal1(ret);
  int displayIndex = Int_val(vDisplay);

  float ddpi, hdpi, vdpi;
  SDL_GetDisplayDPI(displayIndex, &ddpi, &hdpi, &vdpi);

  ret = caml_alloc(3, 0);
  Store_double_field(ret, 0, ddpi);
  Store_double_field(ret, 1, hdpi);
  Store_double_field(ret, 2, vdpi);
  CAMLreturn(ret);
};

CAMLprim value resdl_SDL_GetCurrentDisplayMode(value vDisplay) {
  CAMLparam1(vDisplay);
  CAMLlocal1(ret);

  int displayIndex = Int_val(vDisplay);
  SDL_DisplayMode current;

  SDL_GetCurrentDisplayMode(displayIndex, &current);

  ret = caml_alloc(3, 0);
  Store_field(ret, 0, current.w);
  Store_field(ret, 1, current.h);
  Store_field(ret, 2, current.refresh_rate);
  CAMLreturn(ret);
};

CAMLprim value resdl_SDL_GetDesktopDisplayMode(value vDisplay) {
  CAMLparam1(vDisplay);
  CAMLlocal1(ret);

  int displayIndex = Int_val(vDisplay);
  SDL_DisplayMode current;

  SDL_GetDesktopDisplayMode(displayIndex, &current);

  ret = caml_alloc(3, 0);
  Store_field(ret, 0, current.w);
  Store_field(ret, 1, current.h);
  Store_field(ret, 2, current.refresh_rate);
  CAMLreturn(ret);
};

CAMLprim value resdl_SDL_GetDisplayBounds(value vDisplay) {
  CAMLparam1(vDisplay);
  CAMLlocal1(rect);

  int displayIndex = Int_val(vDisplay);
  SDL_Rect sdlRect;

  SDL_GetDisplayBounds(displayIndex, &sdlRect);

  rect = caml_alloc(4, 0);
  Store_field(rect, 0, Val_int(sdlRect.x));
  Store_field(rect, 1, Val_int(sdlRect.y));
  Store_field(rect, 2, Val_int(sdlRect.w));
  Store_field(rect, 3, Val_int(sdlRect.h));
  CAMLreturn(rect);
};

CAMLprim value resdl_SDL_GetDisplayUsableBounds(value vDisplay) {
  CAMLparam1(vDisplay);
  CAMLlocal1(rect);

  int displayIndex = Int_val(vDisplay);
  SDL_Rect sdlRect;

  SDL_GetDisplayUsableBounds(displayIndex, &sdlRect);

  rect = caml_alloc(4, 0);
  Store_field(rect, 0, Val_int(sdlRect.x));
  Store_field(rect, 1, Val_int(sdlRect.y));
  Store_field(rect, 2, Val_int(sdlRect.w));
  Store_field(rect, 3, Val_int(sdlRect.h));
  CAMLreturn(rect);
};

CAMLprim value resdl_SDL_GetNumVideoDisplays(value vUnit) {
  CAMLparam0();
  int num = SDL_GetNumVideoDisplays();
  CAMLreturn(Val_int(num));
};

CAMLprim value resdl_SDL_GetPixelFormatName(value vPixelFormat) {
  CAMLparam1(vPixelFormat);
  CAMLlocal1(ret);

  Uint32 format = Int_val(vPixelFormat);
  const char *szPixelFormatName = SDL_GetPixelFormatName(format);

  if (!szPixelFormatName) {
    ret = caml_copy_string("(null)");
  } else {
    ret = caml_copy_string(szPixelFormatName);
  }

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GetWindowDisplayIndex(value w) {
  CAMLparam1(w);
  SDL_Window *win = (SDL_Window *)w;
  int idx = SDL_GetWindowDisplayIndex(win);
  CAMLreturn(Val_int(idx));
};

CAMLprim value resdl_SDL_GetWindowPixelFormat(value vWin) {
  CAMLparam1(vWin);
  SDL_Window *pWin = (SDL_Window *)vWin;
  Uint32 format = SDL_GetWindowPixelFormat(pWin);
  CAMLreturn(Val_int(format));
};

CAMLprim value resdl_SDL_GL_SetSwapInterval(value vInterval) {
  int interval = Int_val(vInterval);
  SDL_GL_SetSwapInterval(interval);
  return Val_unit;
};

CAMLprim value resdl_SDL_GL_Setup(value w) {
  SDL_Window *win = (SDL_Window *)w;
  SDL_GLContext ctx = SDL_GL_CreateContext(win);

  if (!ctx) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_GL_Setup failed: %s\n",
                    SDL_GetError());
  }

  return (value)ctx;
}

typedef const GLubyte *(*glGetStringFunc)(GLenum);

CAMLprim value resdl_SDL_GL_GetString(value vStr) {
  CAMLparam1(vStr);
  CAMLlocal1(ret);

  GLenum name = GL_VENDOR;
  switch (Int_val(vStr)) {
  case 0:
    name = GL_VENDOR;
    break;
  case 1:
    name = GL_RENDERER;
    break;
  case 2:
    name = GL_VERSION;
    break;
  case 3:
    name = GL_SHADING_LANGUAGE_VERSION;
    break;
  default:
    break;
  }

  glGetStringFunc glGetString =
      (glGetStringFunc)(SDL_GL_GetProcAddress("glGetString"));

  if (!glGetString) {
    ret = caml_copy_string("Unable to get OpenGL proc address for glGetString");
  } else {
    const char *sz = (const char *)((void *)glGetString(name));
    if (!sz) {
      ret = caml_copy_string("(null)");
    } else {
      ret = caml_copy_string(sz);
    }
  }

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GL_MakeCurrent(value vWindow, value vContext) {
  CAMLparam2(vWindow, vContext);
  SDL_Window *win = (SDL_Window *)vWindow;
  SDL_GLContext ctx = (SDL_GLContext)vContext;

  SDL_GL_MakeCurrent(win, ctx);
  CAMLreturn(Val_unit);
};

CAMLprim value resdl_SDL_GetClipboardText(value vUnit) {
  CAMLparam0();
  CAMLlocal1(ret);

  char *clip = SDL_GetClipboardText();
  if (clip == NULL) {
    ret = Val_none;
  } else {
    ret = Val_some(caml_copy_string(clip));
  }

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_HasClipboardText(value vUnit) {
  CAMLparam0();
  SDL_bool res = SDL_HasClipboardText();
  CAMLreturn(res == SDL_TRUE ? Val_bool(1) : Val_bool(0));
}

CAMLprim value resdl_SDL_SetClipboardText(value vText) {
  CAMLparam1(vText);
  SDL_SetClipboardText(String_val(vText));
  CAMLreturn(Val_unit);
}

CAMLprim value Val_SDL_WindowEvent(int type, int windowID) {
  CAMLparam0();
  CAMLlocal2(ret, v);

  v = caml_alloc(1, 0);
  Store_field(v, 0, Val_int(windowID));

  ret = caml_alloc(1, type);
  Store_field(ret, 0, v);

  CAMLreturn(ret);
}

CAMLprim value Val_SDL_WindowEventWithData(int type, int windowID, int data1,
                                           int data2) {
  CAMLparam0();
  CAMLlocal2(ret, v);

  v = caml_alloc(3, 0);
  Store_field(v, 0, Val_int(windowID));
  Store_field(v, 1, Val_int(data1));
  Store_field(v, 2, Val_int(data2));

  ret = caml_alloc(1, type);
  Store_field(ret, 0, v);

  CAMLreturn(ret);
}

void getNonFocusedMousePosition(SDL_Window *window, int *localMouseX, int *localMouseY) {
  int globalMouseX, globalMouseY;
  SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY);
  int windowX, windowY;
  SDL_GetWindowPosition(window, &windowX, &windowY);
  *localMouseX = globalMouseX - windowX;
  *localMouseY = globalMouseY - windowY;
}

CAMLprim value Val_SDL_Event(SDL_Event *event) {
  CAMLparam0();
  CAMLlocal2(v, vInner);

  int tag, mouseButton, mousePosX, mousePosY;

  switch (event->type) {
  case SDL_QUIT:
    v = Val_int(0);
    break;
  case SDL_MOUSEMOTION:
    vInner = caml_alloc(3, 0);
    Store_field(vInner, 0, Val_int(event->motion.windowID));
    Store_field(vInner, 1, Val_int(event->motion.x));
    Store_field(vInner, 2, Val_int(event->motion.y));

    v = caml_alloc(1, 0);
    Store_field(v, 0, vInner);
    break;
  case SDL_MOUSEWHEEL:
    v = caml_alloc(1, 1);

    vInner = caml_alloc(4, 0);
    Store_field(vInner, 0, Val_int(event->wheel.windowID));
    Store_field(vInner, 1, Val_int(event->wheel.x));
    Store_field(vInner, 2, Val_int(event->wheel.y));
    Store_field(vInner, 3,
                Val_bool(event->wheel.direction == SDL_MOUSEWHEEL_FLIPPED));

    Store_field(v, 0, vInner);
    break;
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    if (event->type == SDL_MOUSEBUTTONDOWN)
      v = caml_alloc(1, 2);
    else
      v = caml_alloc(1, 3);

    mouseButton = 0;
    switch (event->button.button) {
    case SDL_BUTTON_LEFT:
      mouseButton = 0;
      break;
    case SDL_BUTTON_MIDDLE:
      mouseButton = 1;
      break;
    case SDL_BUTTON_RIGHT:
      mouseButton = 2;
      break;
    case SDL_BUTTON_X1:
      mouseButton = 3;
      break;
    case SDL_BUTTON_X2:
      mouseButton = 4;
      break;
    default:
      mouseButton = 0;
      break;
    }

    vInner = caml_alloc(5, 0);
    Store_field(vInner, 0, Val_int(event->button.windowID));
    Store_field(vInner, 1, Val_int(mouseButton));
    Store_field(vInner, 2, Val_int(event->button.clicks));
    Store_field(vInner, 3, Val_int(event->button.x));
    Store_field(vInner, 4, Val_int(event->button.y));

    Store_field(v, 0, vInner);
    break;
  case SDL_KEYDOWN:
  case SDL_KEYUP:
    v = caml_alloc(1, event->type == SDL_KEYDOWN ? 4 : 5);

    vInner = caml_alloc(5, 0);
    Store_field(vInner, 0, Val_int(event->key.windowID));
    Store_field(vInner, 1, Val_bool(event->key.repeat));
    Store_field(vInner, 2, Val_int(event->key.keysym.mod));
    Store_field(vInner, 3, Val_int(event->key.keysym.scancode));
    Store_field(vInner, 4, Val_int(event->key.keysym.sym));

    Store_field(v, 0, vInner);
    break;
  case SDL_TEXTINPUT:
    v = caml_alloc(1, 6);

    vInner = caml_alloc(2, 0);
    Store_field(vInner, 0, Val_int(event->text.windowID));
    Store_field(vInner, 1, caml_copy_string(event->text.text));

    Store_field(v, 0, vInner);
    break;
  case SDL_TEXTEDITING:
    v = caml_alloc(1, 7);

    vInner = caml_alloc(4, 0);
    Store_field(vInner, 0, Val_int(event->edit.windowID));
    Store_field(vInner, 1, caml_copy_string(event->edit.text));
    Store_field(vInner, 2, Val_int(event->edit.start));
    Store_field(vInner, 3, Val_int(event->edit.length));

    Store_field(v, 0, vInner);
    break;
  case SDL_PANEVENT:
    v = caml_alloc(1, 24);

    vInner = caml_alloc(9, 0);
    Store_field(vInner, 0, Val_int(event->window.windowID));
    Store_field(vInner, 1, Val_int(event->pan.x));
    Store_field(vInner, 2, Val_int(event->pan.y));
    Store_field(vInner, 3, Val_bool(event->pan.contains_x));
    Store_field(vInner, 4, Val_bool(event->pan.contains_y));
    Store_field(vInner, 5, Val_bool(event->pan.fling));
    Store_field(vInner, 6, Val_bool(event->pan.interrupt));
    // verify this is the correct way of representing a ref to some WheelType.t
    Store_field(vInner, 7, Val_int(event->pan.source_type));
    Store_field(vInner, 8, Val_int(event->pan.timestamp));

    Store_field(v, 0, vInner);
    break;
  case SDL_DROPTEXT:
    v = caml_alloc(1, 25);
    vInner = caml_alloc(5, 0);

    getNonFocusedMousePosition(SDL_GetWindowFromID(event->drop.windowID), &mousePosX, &mousePosY);

    Store_field(vInner, 0, Val_int(event->drop.windowID));
    Store_field(vInner, 1, caml_copy_string(event->drop.file));
    Store_field(vInner, 2, Val_int(event->drop.timestamp));
    Store_field(vInner, 3, Val_int(mousePosX));
    Store_field(vInner, 4, Val_int(mousePosY));

    Store_field(v, 0, vInner);
    SDL_free(event->drop.file);
    break;
  case SDL_DROPFILE:
    v = caml_alloc(1, 26);
    vInner = caml_alloc(5, 0);

    getNonFocusedMousePosition(SDL_GetWindowFromID(event->drop.windowID), &mousePosX, &mousePosY);

    Store_field(vInner, 0, Val_int(event->drop.windowID));
    Store_field(vInner, 1, caml_copy_string(event->drop.file));
    Store_field(vInner, 2, Val_int(event->drop.timestamp));
    Store_field(vInner, 3, Val_int(mousePosX));
    Store_field(vInner, 4, Val_int(mousePosY));

    Store_field(v, 0, vInner);
    SDL_free(event->drop.file);
    break;
  case SDL_DROPBEGIN:
    v = caml_alloc(1, 27);
    vInner = caml_alloc(4, 0);

    getNonFocusedMousePosition(SDL_GetWindowFromID(event->drop.windowID), &mousePosX, &mousePosY);

    Store_field(vInner, 0, Val_int(event->drop.windowID));
    Store_field(vInner, 1, Val_int(event->drop.timestamp));
    Store_field(vInner, 2, Val_int(mousePosX));
    Store_field(vInner, 3, Val_int(mousePosY));

    Store_field(v, 0, vInner);
    SDL_free(event->drop.file);
    break;
  case SDL_DROPCOMPLETE:
    v = caml_alloc(1, 28);
    vInner = caml_alloc(4, 0);

    getNonFocusedMousePosition(SDL_GetWindowFromID(event->drop.windowID), &mousePosX, &mousePosY);

    Store_field(vInner, 0, Val_int(event->drop.windowID));
    Store_field(vInner, 1, Val_int(event->drop.timestamp));
    Store_field(vInner, 2, Val_int(mousePosX));
    Store_field(vInner, 3, Val_int(mousePosY));

    Store_field(v, 0, vInner);
    SDL_free(event->drop.file);
    break;
  case SDL_KEYMAPCHANGED:
    v = Val_int(2);
    break;
  case SDL_WINDOWEVENT:
    switch (event->window.event) {
    case SDL_WINDOWEVENT_SHOWN:
      v = Val_SDL_WindowEvent(8, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_HIDDEN:
      v = Val_SDL_WindowEvent(9, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_EXPOSED:
      v = Val_SDL_WindowEvent(10, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_MOVED:
      v = Val_SDL_WindowEventWithData(11, event->window.windowID,
                                      event->window.data1, event->window.data2);
      break;
    case SDL_WINDOWEVENT_RESIZED:
      v = Val_SDL_WindowEventWithData(12, event->window.windowID,
                                      event->window.data1, event->window.data2);
      break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      v = Val_SDL_WindowEventWithData(13, event->window.windowID,
                                      event->window.data1, event->window.data2);
      break;
    case SDL_WINDOWEVENT_MINIMIZED:
      v = Val_SDL_WindowEvent(14, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_MAXIMIZED:
      v = Val_SDL_WindowEvent(15, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_FULLSCREEN:
      v = Val_SDL_WindowEvent(29, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_RESTORED:
      v = Val_SDL_WindowEvent(16, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_ENTER:
      v = Val_SDL_WindowEvent(17, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_LEAVE:
      v = Val_SDL_WindowEvent(18, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
      v = Val_SDL_WindowEvent(19, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
      v = Val_SDL_WindowEvent(20, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_CLOSE:
      v = Val_SDL_WindowEvent(21, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_TAKE_FOCUS:
      v = Val_SDL_WindowEvent(22, event->window.windowID);
      break;
    case SDL_WINDOWEVENT_HIT_TEST:
      v = Val_SDL_WindowEvent(23, event->window.windowID);
      break;
    default:
      v = Val_int(1);
    };

    break;
  default:
    v = Val_int(1);
  }

  CAMLreturn(v);
};

CAMLprim value resdl_SDL_PollEvent() {
  CAMLparam0();
  CAMLlocal2(ret, evt);
  SDL_Event e;

  caml_release_runtime_system();
  int result = SDL_PollEvent(&e);
  caml_acquire_runtime_system();

  if (result == 0) {
    ret = Val_none;
  } else {
    evt = Val_SDL_Event(&e);
    ret = Val_some(evt);
  }

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_WaitEvent() {
  CAMLparam0();
  CAMLlocal2(ret, evt);
  SDL_Event e;

  caml_release_runtime_system();
  int result = SDL_WaitEvent(&e);
  caml_acquire_runtime_system();

  if (result == 1) {
    evt = Val_SDL_Event(&e);
    ret = Val_ok(evt);
  } else {
    ret = Val_error(caml_copy_string(SDL_GetError()));
  }

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_PushEvent() {
  CAMLparam0();

  SDL_Event user_event;
  user_event.type = SDL_USEREVENT;
  SDL_PushEvent(&user_event);

  CAMLreturn(Val_unit);
};

CAMLprim value resdl_SDL_WaitTimeoutEvent(value vTimeout) {
  CAMLparam1(vTimeout);
  CAMLlocal2(ret, evt);
  int timeout = Int_val(vTimeout);
  SDL_Event e;

  caml_release_runtime_system();
  int result = SDL_WaitEventTimeout(&e, timeout);
  caml_acquire_runtime_system();

  if (result == 1) {
    evt = Val_SDL_Event(&e);
    ret = Val_some(evt);
  } else {
    ret = Val_none;
  }

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GetTicks() {
  CAMLparam0();

  int result = SDL_GetTicks();

  CAMLreturn(Val_int(result));
}

CAMLprim value resdl_SDL_GetWindowSize(value vWindow) {
  CAMLparam1(vWindow);
  CAMLlocal1(ret);
  SDL_Window *win = (SDL_Window *)vWindow;
  int width, height = 0;
  SDL_GetWindowSize(win, &width, &height);
  ret = caml_alloc(2, 0);
  Store_field(ret, 0, Val_int(width));
  Store_field(ret, 1, Val_int(height));
  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GetWindowPosition(value vWindow) {
  CAMLparam1(vWindow);
  CAMLlocal1(position);

  SDL_Window *win = (SDL_Window *)vWindow;
  int x, y = 0;

  SDL_GetWindowPosition(win, &x, &y);

  position = caml_alloc(2, 0);
  Store_field(position, 0, Val_int(x));
  Store_field(position, 1, Val_int(y));

  CAMLreturn(position);
}

CAMLprim value resdl_SDL_GL_GetDrawableSize(value vWindow) {
  CAMLparam1(vWindow);
  CAMLlocal1(ret);
  SDL_Window *win = (SDL_Window *)vWindow;
  int width, height = 0;
  SDL_GL_GetDrawableSize(win, &width, &height);
  ret = caml_alloc(2, 0);
  Store_field(ret, 0, Val_int(width));
  Store_field(ret, 1, Val_int(height));
  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_SetWindowIcon(value vWindow, value vIcon) {
  CAMLparam2(vWindow, vIcon);

  SDL_Window *win = (SDL_Window *)vWindow;
  SDL_Surface *surface = (SDL_Surface *)vIcon;
  SDL_SetWindowIcon(win, surface);

  CAMLreturn(Val_unit);
};

CAMLprim value resdl_SDL_SetWindowTransparency(value vWindow,
                                               value vTransparency) {
  CAMLparam2(vWindow, vTransparency);

  SDL_Window *win = (SDL_Window *)vWindow;
  double transparency = Double_val(vTransparency);

  int result;
  result = SDL_SetWindowOpacity(win, transparency);

  if (result == -1) {
    printf("WARNING: Setting transparency not supported!");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_CreateSystemCursor(value vCursor) {
  CAMLparam1(vCursor);

  SDL_Cursor *cursor = NULL;

  SDL_SystemCursor id = SDL_SYSTEM_CURSOR_ARROW;
  switch (Int_val(vCursor)) {
  case 1:
    id = SDL_SYSTEM_CURSOR_IBEAM;
    break;
  case 2:
    id = SDL_SYSTEM_CURSOR_WAIT;
    break;
  case 3:
    id = SDL_SYSTEM_CURSOR_CROSSHAIR;
    break;
  case 4:
    id = SDL_SYSTEM_CURSOR_WAITARROW;
    break;
  case 5:
    id = SDL_SYSTEM_CURSOR_SIZENWSE;
    break;
  case 6:
    id = SDL_SYSTEM_CURSOR_SIZENESW;
    break;
  case 7:
    id = SDL_SYSTEM_CURSOR_SIZEWE;
    break;
  case 8:
    id = SDL_SYSTEM_CURSOR_SIZENS;
    break;
  case 9:
    id = SDL_SYSTEM_CURSOR_SIZEALL;
    break;
  case 10:
    id = SDL_SYSTEM_CURSOR_NO;
    break;
  case 11:
    id = SDL_SYSTEM_CURSOR_HAND;
    break;
  default:
    id = SDL_SYSTEM_CURSOR_ARROW;
    break;
  }

  cursor = SDL_CreateSystemCursor(id);
  CAMLreturn((value)cursor);
}

CAMLprim value resdl_SDL_StartTextInput() {
  SDL_StartTextInput();
  return Val_unit;
}

CAMLprim value resdl_SDL_StopTextInput() {
  SDL_StopTextInput();
  return Val_unit;
}

CAMLprim value resdl_SDL_SetTextInputRect(value vX, value vY, value vWidth,
                                          value vHeight) {
  int x = Int_val(vX);
  int y = Int_val(vY);
  int width = Int_val(vWidth);
  int height = Int_val(vHeight);

  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = width;
  rect.h = height;

  SDL_SetTextInputRect(&rect);
  return Val_unit;
}

CAMLprim value resdl_SDL_IsTextInputActive() {
  return Val_bool(SDL_IsTextInputActive());
}

CAMLprim value resdl_SDL_SetCursor(value vCursor) {
  CAMLparam1(vCursor);
  SDL_Cursor *cursor = (SDL_Cursor *)vCursor;
  SDL_SetCursor(cursor);
  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_CreateRGBSurfaceFromImage(value vPath) {
  CAMLparam1(vPath);
  CAMLlocal1(ret);
  // FROM:
  // https://wiki.libsdl.org/SDL_CreateRGBSurfaceFrom

  int req_format = STBI_rgb_alpha;
  int width, height, orig_format;
  unsigned char *data =
      stbi_load(String_val(vPath), &width, &height, &orig_format, req_format);
  if (data == NULL) {

    ret = Val_error(caml_copy_string(stbi_failure_reason()));
  } else {

    // Set up the pixel format color masks for RGB(A) byte arrays.
    // Only STBI_rgb (3) and STBI_rgb_alpha (4) are supported here!
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = (req_format == STBI_rgb) ? 8 : 0;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = (req_format == STBI_rgb) ? 0 : 0xff000000;
#endif

    int depth, pitch;
    if (req_format == STBI_rgb) {
      depth = 24;
      pitch = 3 * width; // 3 bytes per pixel * pixels per row
    } else {             // STBI_rgb_alpha (RGBA)
      depth = 32;
      pitch = 4 * width;
    }

    SDL_Surface *surf = SDL_CreateRGBSurfaceFrom(
        (void *)data, width, height, depth, pitch, rmask, gmask, bmask, amask);
    if (surf == NULL) {
      ret = Val_error(caml_copy_string(SDL_GetError()));
      stbi_image_free(data);
    } else {
      ret = Val_ok((value)surf);
    }
  }

  CAMLreturn(ret);
};

CAMLprim value resdl_SDL_GL_SwapWindow(value w) {
  SDL_Window *win = (SDL_Window *)w;
  caml_release_runtime_system();
  SDL_GL_SwapWindow(win);
  caml_acquire_runtime_system();
  return Val_unit;
}

SDL_HitTestResult hittest(SDL_Window *win, const SDL_Point *area, void *data) {
  return SDL_HITTEST_DRAGGABLE;
}

CAMLprim value resdl_SDL_SetWindowSize(value vWin, value vW, value vH) {
  CAMLparam3(vWin, vW, vH);

  SDL_Window *win = (SDL_Window *)vWin;
  int w = Int_val(vW);
  int h = Int_val(vH);
  SDL_SetWindowSize(win, w, h);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_SetWindowMinimumSize(value vWin, value vW, value vH) {
  CAMLparam3(vWin, vW, vH);

  SDL_Window *win = (SDL_Window *)vWin;
  int w = Int_val(vW);
  int h = Int_val(vH);
  SDL_SetWindowMinimumSize(win, w, h);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_SetWindowTitle(value vWin, value vTitle) {
  CAMLparam2(vWin, vTitle);

  SDL_Window *win = (SDL_Window *)vWin;
  const char *title = (const char *)String_val(vTitle);
  SDL_SetWindowTitle(win, title);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_SetWindowPosition(value vWin, value vX, value vY) {
  CAMLparam3(vWin, vX, vY);

  SDL_Window *win = (SDL_Window *)vWin;
  int x = Int_val(vX);
  int y = Int_val(vY);
  SDL_SetWindowPosition(win, x, y);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_WindowCenter(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_CreateWindow(value vName, value vX, value vY,
                                      value vWidth, value vHeight) {
  CAMLparam5(vName, vX, vY, vWidth, vHeight);

  int x;
  if (vX == hash_variant("Centered")) {
    x = SDL_WINDOWPOS_CENTERED;
  } else if (Is_block(vX) && Field(vX, 0) == hash_variant("Absolute")) {
    x = Int_val(Field(vX, 1));
  } else {
    x = SDL_WINDOWPOS_UNDEFINED;
  };

  int y;
  if (vY == hash_variant("Centered")) {
    y = SDL_WINDOWPOS_CENTERED;
  } else if (Is_block(vY) && Field(vY, 0) == hash_variant("Absolute")) {
    y = Int_val(Field(vY, 1));
  } else {
    y = SDL_WINDOWPOS_UNDEFINED;
  };

  int width = Int_val(vWidth);
  int height = Int_val(vHeight);

  // According to the docs - `SDL_GL_SetAttribute` needs
  // to be called prior to creating the window.

  // Attributes pulled from:
  // https://github.com/google/skia/blob/master/example/SkiaSDLExample.cpp
  static const int kStencilBits = 8; // Skia needs 8 stencil bits
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef WIN32
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif __APPLE__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#else
  // There's no guarantee that Linux 3.0 is available on Linux.
  // ie, on my CentOS 6 box, with latest Intel drivers - only 2.1 is supported.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, kStencilBits);

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  SDL_Window *win = (SDL_CreateWindow(
      String_val(vName), x, y, width, height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE));

  if (!win) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow failed: %s\n",
                    SDL_GetError());
  }

  value vWindow = (value)win;
  CAMLreturn(vWindow);
}

CAMLprim value resdl_SDL_SetWindowBordered(value vWin, value vBordered) {
  CAMLparam2(vWin, vBordered);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_bool bordered = Int_val(vBordered) == 1 ? SDL_TRUE : SDL_FALSE;

  SDL_SetWindowBordered(win, bordered);
  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_SetWindowResizable(value vWin, value vResizable) {
  CAMLparam2(vWin, vResizable);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_bool resize = Int_val(vResizable) == 1 ? SDL_TRUE : SDL_FALSE;

  SDL_SetWindowResizable(win, resize);
  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_HideWindow(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_HideWindow(win);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_MaximizeWindow(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_MaximizeWindow(win);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_IsWindowMaximized(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  Uint32 flags = SDL_GetWindowFlags(win);
  bool hasMaximizedFlag = (flags & SDL_WINDOW_MAXIMIZED) != 0;

  CAMLreturn(Val_bool(hasMaximizedFlag));
}

CAMLprim value resdl_SDL_IsWindowFullscreen(value vWin) {
  CAMLparam1(vWin);
  // SDL's fullscreen window flags don't work on macOS
  SDL_Window *win = (SDL_Window *)vWin;
  bool isFullscreen;
#ifdef __APPLE__
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(win, &wmInfo);
  NSWindow *nWindow = wmInfo.info.cocoa.window;
  isFullscreen = (([nWindow styleMask] & NSWindowStyleMaskFullScreen) == NSWindowStyleMaskFullScreen);
#else
  Uint32 flags = SDL_GetWindowFlags(win);
  isFullscreen = (flags & SDL_WINDOW_FULLSCREEN) != 0;
#endif

  CAMLreturn(Val_bool(isFullscreen));
}

CAMLprim value resdl_SDL_MinimizeWindow(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_MinimizeWindow(win);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_RaiseWindow(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_RaiseWindow(win);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_RestoreWindow(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_RestoreWindow(win);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_ShowWindow(value vWin) {
  CAMLparam1(vWin);

  SDL_Window *win = (SDL_Window *)vWin;
  SDL_ShowWindow(win);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_GetWindowId(value vWindow) {
  CAMLparam1(vWindow);
  SDL_Window *win = (SDL_Window *)vWindow;
  int id = SDL_GetWindowID(win);
  CAMLreturn(Val_int(id));
}

void resdl_onLog(void *unused, int category, SDL_LogPriority priority,
                 const char *message) {
  CAMLparam0();
  CAMLlocal1(messageString);

  static const value *reason_sdl_onLog = NULL;

  if (reason_sdl_onLog == NULL) {
    reason_sdl_onLog = caml_named_value("reason_sdl2_onLog");
  }

  int iCategory, iPriority;

  switch (category) {
  case SDL_LOG_CATEGORY_APPLICATION:
    iCategory = 0;
    break;
  case SDL_LOG_CATEGORY_ERROR:
    iCategory = 1;
    break;
  case SDL_LOG_CATEGORY_ASSERT:
    iCategory = 2;
    break;
  case SDL_LOG_CATEGORY_SYSTEM:
    iCategory = 3;
    break;
  case SDL_LOG_CATEGORY_AUDIO:
    iCategory = 4;
    break;
  case SDL_LOG_CATEGORY_VIDEO:
    iCategory = 5;
    break;
  case SDL_LOG_CATEGORY_RENDER:
    iCategory = 6;
    break;
  case SDL_LOG_CATEGORY_INPUT:
    iCategory = 7;
    break;
  case SDL_LOG_CATEGORY_TEST:
    iCategory = 8;
  case SDL_LOG_CATEGORY_CUSTOM:
    iCategory = 9;
    break;
  default:
    iCategory = 10;
    break;
  }

  switch (priority) {
  case SDL_LOG_PRIORITY_VERBOSE:
    iPriority = 0;
    break;
  case SDL_LOG_PRIORITY_DEBUG:
    iPriority = 1;
    break;
  case SDL_LOG_PRIORITY_INFO:
    iPriority = 2;
    break;
  case SDL_LOG_PRIORITY_WARN:
    iPriority = 3;
    break;
  case SDL_LOG_PRIORITY_ERROR:
    iPriority = 4;
    break;
  case SDL_LOG_PRIORITY_CRITICAL:
    iPriority = 5;
    break;
  default:
    iPriority = 0;
    break;
  }

  messageString = caml_copy_string(message);
  caml_callback3(*reason_sdl_onLog, Val_int(iCategory), Val_int(iPriority),
                 messageString);

  CAMLreturn0;
}

CAMLprim value resdl_SDL_Init() {
  CAMLparam0();

  SDL_LogSetOutputFunction(&resdl_onLog, NULL);

  int ret = SDL_Init(SDL_INIT_VIDEO);

  if (ret < 0) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s\n",
                    SDL_GetError());
  }

  CAMLreturn(Val_int(ret));
}

CAMLprim value resdl_SDL_GetScancodeName(value vScancode) {
  CAMLparam1(vScancode);
  CAMLlocal1(ret);

  SDL_Scancode scancode = (SDL_Scancode)Int_val(vScancode);
  ret = caml_copy_string(SDL_GetScancodeName(scancode));
  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GetKeyFromName(value vKeyName) {
  CAMLparam1(vKeyName);

  int keycode = (int)SDL_GetKeyFromName(String_val(vKeyName));

  CAMLreturn(Val_int(keycode));
}

CAMLprim value resdl_SDL_GetScancodeFromName(value vScancodeName) {
  CAMLparam1(vScancodeName);

  int scancode = (int)SDL_GetScancodeFromName(String_val(vScancodeName));

  CAMLreturn(Val_int(scancode));
}

CAMLprim value resdl_SDL_GetKeyName(value vKey) {
  CAMLparam1(vKey);
  CAMLlocal1(ret);

  SDL_Keycode key = (SDL_Keycode)Int_val(vKey);
  ret = caml_copy_string(SDL_GetKeyName(key));
  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GetKeyFromScancode(value vScancode) {
  SDL_Scancode scancode = (SDL_Scancode)Int_val(vScancode);
  return Val_int(SDL_GetKeyFromScancode(scancode));
}

CAMLprim value resdl_SDL_GetScancodeFromKey(value vKey) {
  SDL_Keycode key = (SDL_Keycode)Int_val(vKey);
  return Val_int(SDL_GetScancodeFromKey(key));
}

CAMLprim value resdl_SDL_ModLeftShift(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_LSHIFT) == KMOD_LSHIFT);
};

CAMLprim value resdl_SDL_ModRightShift(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_RSHIFT) == KMOD_RSHIFT);
};

CAMLprim value resdl_SDL_ModLeftControl(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_LCTRL) == KMOD_LCTRL);
};

CAMLprim value resdl_SDL_ModRightControl(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_RCTRL) == KMOD_RCTRL);
};

CAMLprim value resdl_SDL_ModLeftAlt(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_LALT) == KMOD_LALT);
};

CAMLprim value resdl_SDL_ModRightAlt(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_RALT) == KMOD_RALT);
};

CAMLprim value resdl_SDL_ModLeftGui(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_LGUI) == KMOD_LGUI);
};

CAMLprim value resdl_SDL_ModRightGui(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_RGUI) == KMOD_RGUI);
};

CAMLprim value resdl_SDL_ModNumLockDown(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_NUM) == KMOD_NUM);
};

CAMLprim value resdl_SDL_ModCapsLockDown(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_CAPS) == KMOD_CAPS);
};

CAMLprim value resdl_SDL_ModAltGrDown(value vMod) {
  return Val_bool((Int_val(vMod) & KMOD_MODE) == KMOD_MODE);
};

CAMLprim value resdl_SDL_GetModState(value vUnit) {
  return Val_int(SDL_GetModState());
};

CAMLprim value resdl_SDL_SetModState(value vMod) {
  SDL_SetModState((SDL_Keymod)Int_val(vMod));
  return Val_unit;
};

CAMLprim value resdl_SDL_GetCompiledVersion(value vUnit) {
  CAMLparam0();
  CAMLlocal1(ret);

  SDL_version compiled;
  SDL_VERSION(&compiled);

  ret = caml_alloc(3, 0);
  Store_field(ret, 0, Val_int(compiled.major));
  Store_field(ret, 1, Val_int(compiled.minor));
  Store_field(ret, 2, Val_int(compiled.patch));

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_GetLinkedVersion(value vUnit) {
  CAMLparam0();
  CAMLlocal1(ret);

  SDL_version linked;
  SDL_GetVersion(&linked);

  ret = caml_alloc(3, 0);
  Store_field(ret, 0, Val_int(linked.major));
  Store_field(ret, 1, Val_int(linked.minor));
  Store_field(ret, 2, Val_int(linked.patch));

  CAMLreturn(ret);
}

CAMLprim value resdl_SDL_ShowSimpleMessageBox(value vFlags, value vTitle,
                                              value vMessage, value vWindow) {
  CAMLparam4(vFlags, vTitle, vMessage, vWindow);
  int flags = SDL_MESSAGEBOX_INFORMATION;

  switch (Int_val(vFlags)) {
  case 0:
    flags = SDL_MESSAGEBOX_ERROR;
    break;
  case 1:
    flags = SDL_MESSAGEBOX_WARNING;
    break;
  default:
    flags = SDL_MESSAGEBOX_INFORMATION;
    break;
  }

  const char *title = String_val(vTitle);
  const char *msg = String_val(vMessage);

  SDL_Window *win = NULL;

  if (Is_block(vWindow)) {
    win = (SDL_Window *)Field(vWindow, 0);
  }

  SDL_ShowSimpleMessageBox(flags, title, msg, win);

  CAMLreturn(Val_unit);
}

CAMLprim value resdl_SDL_CaptureMouse(value vEnabled) {
  CAMLparam0();
  SDL_bool enabled = Int_val(vEnabled) == 1 ? SDL_TRUE : SDL_FALSE;
  CAMLreturn(Val_int(SDL_CaptureMouse(enabled)));
}

CAMLprim value resdl_PassThrough(value v) { return v; };

CAMLprim value resdl__javascript__renderloop() { return Val_unit; }
};
