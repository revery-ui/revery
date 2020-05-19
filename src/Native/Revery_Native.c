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
#elif __APPLE__
#include "ReveryCocoa.h"
#import "ReveryAppDelegate.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_initialize() {
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

CAMLprim value revery_uninitialize() {
#ifdef WIN32
    CoUninitialize();
#endif
    return Val_unit;
}

CAMLprim value revery_log(value vStr) {
    CAMLparam1(vStr);
    const char *str = String_val(str);
#ifdef __APPLE__
    NSLog(@"%s", str);
#else
    UNUSED(str);
#endif
    CAMLreturn(Val_unit);
}
