#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#define UNUSED(x) (void)(x)

#ifdef WIN32
#include "ReveryWin32.h"
#include <combaseapi.h>
#include <windows.h>
#elif __APPLE__
#include "ReveryCocoa.h"
#import "ReveryAppDelegate.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_initializeApp() {
#ifdef __APPLE__
    SDLAppDelegate *sdlDelegate = [NSApp delegate];
    ReveryAppDelegate *delegate = [ReveryAppDelegate newWithSDLDelegate:sdlDelegate];
    [NSApp setDelegate:delegate];
#elif WIN32
    HRESULT hr = CoInitialize(NULL);
    if (hr != S_OK) {
        fprintf(stderr, "WARNING: COM initialization failed.");
    }
#endif
    return Val_unit;
}

CAMLprim value revery_uninitializeApp() {
#ifdef WIN32
    CoUninitialize();
#endif
    return Val_unit;
}


CAMLprim value revery_initializeWindow(value vWin) {
    CAMLparam1(vWin);
    void *win = (void *)vWin;
#ifdef WIN32
    HWND window = (HWND)win;
    int current_style = GetWindowLong(window, GWL_STYLE);
    SetWindowLong(window, GWL_STYLE, current_style | WS_CAPTION);
#else
    UNUSED(win);
#endif
    CAMLreturn(Val_unit);
}
