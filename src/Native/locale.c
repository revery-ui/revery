#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#include "caml_values.h"

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_getUserLocale() {
    CAMLparam0();
    char *ret;
#ifdef __APPLE__
    ret = revery_getUserLocale_cocoa();
#endif
    CAMLreturn(caml_copy_string(ret));
}
