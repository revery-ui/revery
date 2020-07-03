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
#include <locale.h>
#endif

CAMLprim value revery_getUserLocale() {
    CAMLparam0();
    CAMLlocal1(camlRet);
    char *ret;
#ifdef __APPLE__
    ret = revery_getUserLocale_cocoa();
    camlRet = caml_copy_string(ret);
#elif WIN32
    ret = revery_getUserLocale_win32();
    camlRet = caml_copy_string(ret);
#else
    setlocale(LC_CTYPE, "");
    ret = setlocale(LC_CTYPE, NULL);
    camlRet = caml_copy_string(ret);
    free(ret);
#endif
    CAMLreturn(camlRet);
}
