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
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_config.h>
#include <SDL2/SDL_syswm.h>

#ifdef SDL_VIDEO_DRIVER_COCOA
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
    /* Create an OCaml value encapsulating the pointer p */
    value resdl_wrapPointer(void *p) {
        value v = caml_alloc(1, Abstract_tag);
        *((void **) Data_abstract_val(v)) = p;
        return v;
    }

    /* Extract the pointer encapsulated in the given OCaml value */
    void *resdl_unwrapPointer(value v) {
        return *((void **) Data_abstract_val(v));
    }

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
        CAMLparam1(vWin);
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_DestroyWindow(win);
        CAMLreturn(Val_unit);
    }

    SDL_HitTestResult resdl_hit_test(SDL_Window *win, const SDL_Point *area,
                                     void *data) {

        CAMLparam0();
        CAMLlocal2(vWin, vRet);
        static const value *hitTestCallback = NULL;
        if (hitTestCallback == NULL) {
            hitTestCallback = caml_named_value("__sdl2_caml_hittest__");
        }
        vWin = resdl_wrapPointer(win);
        vRet = caml_callback3(*hitTestCallback, vWin, Val_int(area->x),
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

        CAMLreturnT(SDL_HitTestResult, result);
    };

    CAMLprim value resdl_SDL_EnableHitTest(value vWin) {
        CAMLparam1(vWin);
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_SetWindowHitTest(win, resdl_hit_test, NULL);
        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_DisableHitTest(value vWin) {
        CAMLparam1(vWin);
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_SetWindowHitTest(win, NULL, NULL);
        CAMLreturn(Val_unit);
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
        CAMLlocal1(vNativeWindow);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(win, &wmInfo);

        void *pNativeWindow = NULL;
        switch (wmInfo.subsystem) {
#ifdef SDL_VIDEO_DRIVER_WINDOWS
        case SDL_SYSWM_WINDOWS:
            pNativeWindow = (void *)wmInfo.info.win.window;
            break;
#endif
#ifdef SDL_VIDEO_DRIVER_UIKIT
        case SDL_SYSWM_UIKIT:
            pNativeWindow = (void *)wmInfo.info.uikit.window;
            break;
#endif
#ifdef SDL_VIDEO_DRIVER_COCOA
        case SDL_SYSWM_COCOA:
            pNativeWindow = (void *)wmInfo.info.cocoa.window;
            break;
#endif
#ifdef SDL_VIDEO_DRIVER_ANDROID
        case SDL_SYSWM_ANDROID:
            pNativeWindow = (void *)wmInfo.info.android.window;
            break;
#endif
#ifdef SDL_VIDEO_DRIVER_X11
        case SDL_SYSWM_X11:
            pNativeWindow = (void *)wmInfo.info.x11.window;
            break;
#endif
#ifdef SDL_VIDEO_DRIVER_WAYLAND
        case SDL_SYSWM_WAYLAND:
            pNativeWindow = (void *)wmInfo.info.wl.surface;
            break;
#endif
        default:
            break;
        }

        vNativeWindow = resdl_wrapPointer(pNativeWindow);

        CAMLreturn(vNativeWindow);
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

    CAMLprim value resdl_SDL_SetMacTitlebarHidden(value vWin) {
        CAMLparam1(vWin);

#ifdef SDL_VIDEO_DRIVER_COCOA
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(win, &wmInfo);
        NSWindow *nWindow = wmInfo.info.cocoa.window;
        [nWindow
         setStyleMask:[nWindow styleMask] | NSWindowStyleMaskDocModalWindow];
#endif

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_SetMacTitlebarTransparent(value vWin) {
        CAMLparam1(vWin);

#ifdef SDL_VIDEO_DRIVER_COCOA
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
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

    CAMLprim value resdl_SDL_GetMacTitlebarHeight(value vWin) {
        CAMLparam1(vWin);
        double titlebarHeight = 0.0;

#ifdef SDL_VIDEO_DRIVER_COCOA
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(win, &wmInfo);
        NSWindow *nWindow = wmInfo.info.cocoa.window;

        // Sourced from: https://stackoverflow.com/a/59323932/12701512
        CGFloat windowFrameHeight = CGRectGetHeight([nWindow contentView].frame);
        CGFloat contentLayoutRectHeight = CGRectGetHeight([nWindow contentLayoutRect]);
        titlebarHeight = (double)(windowFrameHeight - contentLayoutRectHeight);
#endif
        CAMLreturn(caml_copy_double(titlebarHeight));
    }

    CAMLprim value resdl_SDL_SetMacBackgroundColor(value vWin, value r, value g,
            value b, value a) {
        CAMLparam5(vWin, r, g, b, a);

#ifdef SDL_VIDEO_DRIVER_COCOA
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
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
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
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

        ret = caml_alloc(4, 0);
        Store_field(ret, 0, Val_int(current.format));
        Store_field(ret, 1, Val_int(current.w));
        Store_field(ret, 2, Val_int(current.h));
        Store_field(ret, 3, Val_int(current.refresh_rate));
        CAMLreturn(ret);
    };

    CAMLprim value resdl_SDL_GetDesktopDisplayMode(value vDisplay) {
        CAMLparam1(vDisplay);
        CAMLlocal1(ret);

        int displayIndex = Int_val(vDisplay);
        SDL_DisplayMode current;

        SDL_GetDesktopDisplayMode(displayIndex, &current);

        ret = caml_alloc(4, 0);
        Store_field(ret, 0, Val_int(current.format));
        Store_field(ret, 1, Val_int(current.w));
        Store_field(ret, 2, Val_int(current.h));
        Store_field(ret, 3, Val_int(current.refresh_rate));
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

    CAMLprim value resdl_SDL_GetDisplayName(value vDisplay) {
        CAMLparam1(vDisplay);
        CAMLlocal1(retStr);

        int displayIndex = Int_val(vDisplay);
        const char *szDisplayName = SDL_GetDisplayName(displayIndex);
        if (!szDisplayName) {
            szDisplayName = "(Null)";
        };

        retStr = caml_copy_string(szDisplayName);
        CAMLreturn(retStr);
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

    CAMLprim value resdl_SDL_GetWindowDisplayIndex(value vWin) {
        CAMLparam1(vWin);
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        int idx = SDL_GetWindowDisplayIndex(win);
        CAMLreturn(Val_int(idx));
    };

    CAMLprim value resdl_SDL_GetWindowPixelFormat(value vWin) {
        CAMLparam1(vWin);
        SDL_Window *pWin = (SDL_Window *)resdl_unwrapPointer(vWin);
        Uint32 format = SDL_GetWindowPixelFormat(pWin);
        CAMLreturn(Val_int(format));
    };

    CAMLprim value resdl_SDL_GL_SetSwapInterval(value vInterval) {
        int interval = Int_val(vInterval);
        SDL_GL_SetSwapInterval(interval);
        return Val_unit;
    };

    CAMLprim value resdl_SDL_GL_Setup(value vWin) {
        CAMLparam1(vWin);
        CAMLlocal1(vCtx);
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_GLContext ctx = SDL_GL_CreateContext(win);

        if (!ctx) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_GL_Setup failed: %s\n",
                            SDL_GetError());
        }

        vCtx = resdl_wrapPointer(ctx);

        CAMLreturn(vCtx);
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

    typedef void (*glGetIntegervFunc)(GLenum, GLint*);
    CAMLprim value resdl_SDL_GL_GetFramebufferBinding(value vInt) {
        CAMLparam1(vInt);

        GLenum name = GL_FRAMEBUFFER_BINDING;
        switch (Int_val(vInt)) {
        case 0:
            name = GL_FRAMEBUFFER_BINDING;
            break;
        default:
            break;
        }

        GLint ret = -1;
        glGetIntegervFunc glGetIntegerv =
            (glGetIntegervFunc)(SDL_GL_GetProcAddress("glGetIntegerv"));
        if (glGetIntegerv) {
            glGetIntegerv(name, &ret);
        }
        CAMLreturn(Val_int(ret));
    }

    CAMLprim value resdl_SDL_GL_MakeCurrent(value vWin, value vContext) {
        CAMLparam2(vWin, vContext);
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);

        SDL_GLContext ctx = (SDL_GLContext)resdl_unwrapPointer(vContext);

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
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWindow);
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

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWindow);
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
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWindow);
        int width, height = 0;
        SDL_GL_GetDrawableSize(win, &width, &height);
        ret = caml_alloc(2, 0);
        Store_field(ret, 0, Val_int(width));
        Store_field(ret, 1, Val_int(height));
        CAMLreturn(ret);
    }

    CAMLprim value resdl_SDL_SetWindowIcon(value vWindow, value vIcon) {
        CAMLparam2(vWindow, vIcon);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWindow);
        SDL_Surface *surface = (SDL_Surface *)resdl_unwrapPointer(vIcon);
        SDL_SetWindowIcon(win, surface);

        CAMLreturn(Val_unit);
    };

    CAMLprim value resdl_SDL_SetWindowTransparency(value vWindow,
            value vTransparency) {
        CAMLparam2(vWindow, vTransparency);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWindow);
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
        CAMLlocal1(vCursorRet);

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
        vCursorRet = resdl_wrapPointer(cursor);
        CAMLreturn(vCursorRet);
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
        SDL_Cursor *cursor = (SDL_Cursor *)resdl_unwrapPointer(vCursor);
        SDL_SetCursor(cursor);
        CAMLreturn(Val_unit);
    }

    static SDL_AudioFormat SDL_AudioFormat_val(value tag) {
        switch (Int_val(tag)) {
        case 0:
            return AUDIO_S8;
        case 1:
            return AUDIO_U8;
        case 2:
            return AUDIO_S16LSB;
        case 3:
            return AUDIO_S16MSB;
        case 4:
            return AUDIO_U16LSB;
        case 5:
            return AUDIO_U16MSB;
        case 6:
            return AUDIO_S32LSB;
        case 7:
            return AUDIO_S32MSB;
        case 8:
            return AUDIO_F32LSB;
        case 9:
            return AUDIO_F32MSB;
        default:
            return 0;
        }
    }

    CAMLprim value Val_SDL_AudioFormat(SDL_AudioFormat format) {
        CAMLparam0();
        int tag;
        switch (format) {
        case AUDIO_S8:
            tag = 0;
            break;
        case AUDIO_U8:
            tag = 1;
            break;
        case AUDIO_S16LSB:
            tag = 2;
            break;
        case AUDIO_S16MSB:
            tag = 3;
            break;
        case AUDIO_U16LSB:
            tag = 4;
            break;
        case AUDIO_U16MSB:
            tag = 5;
            break;
        case AUDIO_S32LSB:
            tag = 6;
            break;
        case AUDIO_S32MSB:
            tag = 7;
            break;
        case AUDIO_F32LSB:
            tag = 8;
            break;
        case AUDIO_F32MSB:
            tag = 9;
            break;
        }
        CAMLreturn(Val_int(tag));
    }

    CAMLprim value Val_SDL_AudioSpec(SDL_AudioSpec spec) {
        CAMLparam0();
        CAMLlocal1(ret);
        ret = caml_alloc(7, 0);
        Store_field(ret, 0, Val_int(spec.freq));
        Store_field(ret, 1, Val_SDL_AudioFormat(spec.format));
        Store_field(ret, 2, Val_int(spec.channels));
        Store_field(ret, 3, Val_int(spec.silence));
        Store_field(ret, 4, Val_int(spec.samples));
        Store_field(ret, 5, Val_int(spec.padding));
        Store_field(ret, 6, Val_int(spec.size));
        CAMLreturn(ret);
    }

    CAMLprim value resdl_SDL_LoadWAV(value vPath) {
        CAMLparam1(vPath);
        CAMLlocal3(ret, vSpec, vTup3);
        SDL_AudioSpec spec;
        Uint8 *audioBuf;
        Uint32 audioLen;
        SDL_LoadWAV(String_val(vPath), &spec, &audioBuf, &audioLen);
        if (audioBuf == NULL) {
            ret = Val_error(caml_copy_string(SDL_GetError()));
            CAMLreturn(ret);
        }
        vSpec = Val_SDL_AudioSpec(spec);
        vTup3 = caml_alloc_tuple(3);
        Store_field(vTup3, 0, vSpec);
        Store_field(vTup3, 1, resdl_wrapPointer(audioBuf));
        Store_field(vTup3, 2, Val_int(audioLen));
        ret = Val_ok(vTup3);
        CAMLreturn(ret);
    }

    CAMLprim value resdl_SDL_OpenAudioDevice(
        value vDeviceNameOpt,
        value vIsCapture,
        value vWant,
        value vAllowedChanges) {
        CAMLparam4(vDeviceNameOpt, vIsCapture, vWant, vAllowedChanges);
        CAMLlocal3(ret, vHave, vTup2);
        const char *deviceName = Is_block(vDeviceNameOpt)
                                 ? String_val(Field(vDeviceNameOpt, 0))
                                 : NULL;
        int isCapture = Bool_val(vIsCapture);
        SDL_AudioSpec want = {0};
        want.freq = Int_val(Field(vWant, 0));
        want.format = SDL_AudioFormat_val(Field(vWant, 1));
        want.channels = Int_val(Field(vWant, 2));
        want.silence = Int_val(Field(vWant, 3));
        want.samples = Int_val(Field(vWant, 4));
        want.padding = Int_val(Field(vWant, 5));
        want.size = Int_val(Field(vWant, 6));
        SDL_AudioSpec have = {0};
        int allowedChanges = Int_val(vAllowedChanges);
        SDL_AudioDeviceID device = SDL_OpenAudioDevice(
                                       deviceName,
                                       isCapture,
                                       &want,
                                       &have,
                                       allowedChanges);
        if (device == 0) {
            ret = Val_error(caml_copy_string(SDL_GetError()));
            CAMLreturn(ret);
        }
        vHave = Val_SDL_AudioSpec(have);
        vTup2 = caml_alloc_tuple(2);
        Store_field(vTup2, 0, Val_int(device));
        Store_field(vTup2, 1, vHave);
        ret = Val_ok(vTup2);
        CAMLreturn(ret);
    }

    CAMLprim value resdl_SDL_CloseAudioDevice(value vDevice) {
        CAMLparam1(vDevice);
        SDL_AudioDeviceID device = Int_val(vDevice);
        SDL_CloseAudioDevice(device);
        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_GetAudioDeviceStatus(value vDevice) {
        CAMLparam1(vDevice);
        SDL_AudioDeviceID device = Int_val(vDevice);
        SDL_AudioStatus status = SDL_GetAudioDeviceStatus(device);
        CAMLreturn(Val_int(status));
    }

    CAMLprim value resdl_SDL_PauseAudioDevice(value vDevice, value vPauseOn) {
        CAMLparam2(vDevice, vPauseOn);
        SDL_AudioDeviceID device = Int_val(vDevice);
        int pauseOn = Bool_val(vPauseOn);
        SDL_PauseAudioDevice(device, pauseOn);
        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_QueueAudio(value vDevice, value vBuf, value vLen) {
        CAMLparam3(vDevice, vBuf, vLen);
        CAMLlocal1(ret);
        SDL_AudioDeviceID device = Int_val(vDevice);
        const void *buf = resdl_unwrapPointer(vBuf);
        Uint32 len = Int_val(vLen);
        if (SDL_QueueAudio(device, buf, len) < 0) {
            ret = Val_error(caml_copy_string(SDL_GetError()));
            CAMLreturn(ret);
        }
        ret = Val_ok(Val_unit);
        CAMLreturn(ret);
    }

    CAMLprim value resdl_SDL_ClearQueuedAudio(value vDevice) {
        CAMLparam1(vDevice);
        SDL_AudioDeviceID device = Int_val(vDevice);
        SDL_ClearQueuedAudio(device);
        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_GetQueuedAudioSize(value vDevice) {
        CAMLparam1(vDevice);
        SDL_AudioDeviceID device = Int_val(vDevice);
        Uint32 bytes = SDL_GetQueuedAudioSize(device);
        CAMLreturn(Val_int(bytes));
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
                ret = Val_ok(resdl_wrapPointer(surf));
            }
        }

        CAMLreturn(ret);
    };

    CAMLprim value resdl_SDL_GL_SwapWindow(value vWin) {
        CAMLparam1(vWin);
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        caml_release_runtime_system();
        SDL_GL_SwapWindow(win);
        caml_acquire_runtime_system();
        CAMLreturn(Val_unit);
    }

    SDL_HitTestResult hittest(SDL_Window *win, const SDL_Point *area, void *data) {
        return SDL_HITTEST_DRAGGABLE;
    }

    CAMLprim value resdl_SDL_SetWindowSize(value vWin, value vW, value vH) {
        CAMLparam3(vWin, vW, vH);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        int w = Int_val(vW);
        int h = Int_val(vH);
        SDL_SetWindowSize(win, w, h);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_SetWindowMinimumSize(value vWin, value vW, value vH) {
        CAMLparam3(vWin, vW, vH);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        int w = Int_val(vW);
        int h = Int_val(vH);
        SDL_SetWindowMinimumSize(win, w, h);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_SetWindowTitle(value vWin, value vTitle) {
        CAMLparam2(vWin, vTitle);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        const char *title = (const char *)String_val(vTitle);
        SDL_SetWindowTitle(win, title);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_SetWindowPosition(value vWin, value vX, value vY) {
        CAMLparam3(vWin, vX, vY);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        int x = Int_val(vX);
        int y = Int_val(vY);
        SDL_SetWindowPosition(win, x, y);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_WindowCenter(value vWin) {
        CAMLparam1(vWin);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_CreateWindow(value vName, value vX, value vY,
                                          value vWidth, value vHeight, value vAcceleration) {
        CAMLparam5(vName, vX, vY, vWidth, vHeight);
        CAMLxparam1(vAcceleration);
        CAMLlocal1(vWindow);

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
#ifdef WIN32
        SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#elif SDL_VIDEO_OPENGL
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

#ifdef SDL_VIDEO_DRIVER_X11
        // Disable compositing suppression - https://github.com/onivim/oni2/issues/2003
        SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, kStencilBits);

        // If vAcceleration is Auto, don't set SDL_GL_ACCELERATED_VISUAL at all.
        if (vAcceleration == hash_variant("ForceHardware")) {
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        } else if (vAcceleration == hash_variant("ForceSoftware")) {
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 0);
        }

        SDL_Window *win = (SDL_CreateWindow(
                               String_val(vName), x, y, width, height,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE));

        if (!win) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow failed: %s\n",
                            SDL_GetError());
        }

        vWindow = resdl_wrapPointer(win);
        CAMLreturn(vWindow);
    }

    CAMLprim value resdl_SDL_CreateWindow_byte(value *argv, int argn) {
        return resdl_SDL_CreateWindow(
                   argv[0],
                   argv[1],
                   argv[2],
                   argv[3],
                   argv[4],
                   argv[5]
               );
    }

    CAMLprim value resdl_SDL_SetWindowBordered(value vWin, value vBordered) {
        CAMLparam2(vWin, vBordered);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_bool bordered = Int_val(vBordered) == 1 ? SDL_TRUE : SDL_FALSE;

        SDL_SetWindowBordered(win, bordered);
        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_SetWindowResizable(value vWin, value vResizable) {
        CAMLparam2(vWin, vResizable);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_bool resize = Int_val(vResizable) == 1 ? SDL_TRUE : SDL_FALSE;

        SDL_SetWindowResizable(win, resize);
        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_HideWindow(value vWin) {
        CAMLparam1(vWin);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_HideWindow(win);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_MaximizeWindow(value vWin) {
        CAMLparam1(vWin);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_MaximizeWindow(win);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_IsWindowMaximized(value vWin) {
        CAMLparam1(vWin);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        Uint32 flags = SDL_GetWindowFlags(win);
        bool hasMaximizedFlag = (flags & SDL_WINDOW_MAXIMIZED) != 0;

        CAMLreturn(Val_bool(hasMaximizedFlag));
    }

    CAMLprim value resdl_SDL_IsWindowFullscreen(value vWin) {
        CAMLparam1(vWin);
        // SDL's fullscreen window flags don't work on macOS
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        bool isFullscreen;
#ifdef SDL_VIDEO_DRIVER_COCOA
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

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_MinimizeWindow(win);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_RaiseWindow(value vWin) {
        CAMLparam1(vWin);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_RaiseWindow(win);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_RestoreWindow(value vWin) {
        CAMLparam1(vWin);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_RestoreWindow(win);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_ShowWindow(value vWin) {
        CAMLparam1(vWin);

        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWin);
        SDL_ShowWindow(win);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_GetWindowId(value vWindow) {
        CAMLparam1(vWindow);
        SDL_Window *win = (SDL_Window *)resdl_unwrapPointer(vWindow);
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

        int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

        if (ret < 0) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s\n",
                            SDL_GetError());
        }

        CAMLreturn(Val_int(ret));
    }

#if SDL_VIDEO_DRIVER_UIKIT
    value resdl_uikit_main_callback;
    int resdl_uikit_SDL_main (int argc, char *argv[]) {
        caml_callback(resdl_uikit_main_callback, Val_unit);
        return 0;
    }
#endif

    CAMLprim value resdl_SDL_main(value ml_argc, value ml_argv, value closure) {
        CAMLparam3(ml_argc, ml_argv, closure);
        CAMLlocal1(tmp);

#if SDL_VIDEO_DRIVER_UIKIT
        int argc = Int_val(ml_argc);
        char **argv = (char**)calloc(sizeof(char*), argc + 1);
        for (int i = 0; i < argc; i++) {
            argv[i] = (char*)String_val(Field(ml_argv, i));
        }

        resdl_uikit_main_callback = closure;
        SDL_UIKitRunApp(argc, argv, resdl_uikit_SDL_main);
#else
        caml_callback(closure, Val_unit);
#endif

        CAMLreturn(Val_unit);
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
            win = (SDL_Window *)resdl_unwrapPointer(Field(vWindow, 0));
        }

        SDL_ShowSimpleMessageBox(flags, title, msg, win);

        CAMLreturn(Val_unit);
    }

    CAMLprim value resdl_SDL_CaptureMouse(value vEnabled) {
        CAMLparam0();
        SDL_bool enabled = Int_val(vEnabled) == 1 ? SDL_TRUE : SDL_FALSE;
        CAMLreturn(Val_int(SDL_CaptureMouse(enabled)));
    }

    CAMLprim value resdl_SDL_GetGlobalMouseState(value vUnit) {
        CAMLparam0();
        CAMLlocal1(ret);

        int x, y;
        SDL_GetGlobalMouseState(&x, &y);

        ret = caml_alloc(2, 0);
        Store_field(ret, 0, Val_int(x));
        Store_field(ret, 1, Val_int(y));

        CAMLreturn(ret);
    }

    CAMLprim value resdl_PassThrough(value v) {
        return v;
    };

    CAMLprim value resdl__javascript__renderloop() {
        return Val_unit;
    }
};
