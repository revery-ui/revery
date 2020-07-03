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
    int shouldFree;
#ifdef __APPLE__
    ret = revery_getUserLocale_cocoa();
    shouldFree = 0;
#elif WIN32
    ret = revery_getUserLocale_win32();
    shouldFree = 1;
#else
    setlocale(LC_CTYPE, "");
    ret = setlocale(LC_CTYPE, NULL);
    shouldFree = 0;
#endif
    camlRet = caml_copy_string(ret);
    if (shouldFree) {
        free(ret);
    }
    CAMLreturn(camlRet);
}
