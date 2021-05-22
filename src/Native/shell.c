#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "caml_values.h"

#include "config.h"
#ifdef USE_WIN32
#include "ReveryWin32.h"
#elif USE_COCOA
#include "ReveryCocoa.h"
#elif USE_GTK
#include "ReveryGtk.h"
#endif
#include "utilities.h"

CAMLprim value revery_openURL(value vURL) {
    CAMLparam1(vURL);

    const char *url_string = String_val(vURL);
    int success = 0;
#ifdef USE_COCOA
    success = revery_openURL_cocoa(url_string);
#elif USE_GTK
    success = revery_openURL_gtk(url_string);
#elif USE_WIN32
    success = revery_openURL_win32(url_string);
#else
    fprintf(stderr, "WARNING: %s is not implemented on this platform.\n", __func__);
    success = 0;
    UNUSED(url_string);
#endif
    CAMLreturn(Val_bool(success));
}

CAMLprim value revery_openFile(value vPath) {
    CAMLparam1(vPath);

    const char *path_string = String_val(vPath);
    int success = 0;
#ifdef USE_COCOA
    success = revery_openFile_cocoa(path_string);
#elif USE_GTK
    success = revery_openFile_gtk(path_string);
#elif USE_WIN32
    // The Win32 implementation of the URL opener also works for file paths
    success = revery_openURL_win32(path_string);
#else
    fprintf(stderr, "WARNING: %s is not implemented on this platform.\n", __func__);
    success = 0;
    UNUSED(path_string);
#endif
    CAMLreturn(Val_bool(success));
}
