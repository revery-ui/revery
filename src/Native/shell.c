#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "caml_values.h"

#define UNUSED(x) (void)(x)

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_openItemAtURL(value vURL) {
    CAMLparam1(vURL);

    const char *url_string = String_val(vURL);
    int success = 0;
#ifdef __APPLE__
    success = revery_openItemAtURL_cocoa(url_string);
#elif __linux__
    success = revery_openItemAtURL_gtk(url_string);
#else
    fprintf(stderr, "WARNING: %s is not implemented on this platform.\n", __func__);
    success = 0;
    UNUSED(url_string);
#endif
    CAMLreturn(Val_bool(success));
}
