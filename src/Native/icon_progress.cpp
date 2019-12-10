#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

#define Val_none Val_int(0)
static value Val_some(value v) {
    CAMLparam1(v);
    CAMLlocal1(some);
    some = caml_alloc(1, 0);
    Store_field(some, 0, v);
    CAMLreturn(some);
}

#define Some_val(v) Field(v, 0)

extern "C" {

CAMLprim value revery_registerIconProgress() {
    CAMLparam0();
    void *ret;
#ifdef __APPLE__
    ret = revery_register_icon_progress_cocoa();
#elif WIN32
    ret = revery_register_icon_progress_win32();
#endif
    CAMLreturn((value)ret);
}

CAMLprim value revery_setIconProgress(value vIconProgress, value vProgress) {
    CAMLparam2(vIconProgress, vProgress);
    void *ip = (void *)vIconProgress;
    double progress = Double_val(vProgress);

#ifdef __APPLE__
    revery_set_icon_progress_cocoa(ip, progress);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_deregisterIconProgress(value vIconProgress) {
    CAMLparam1(vIconProgress);
    void *ip = (void *)vIconProgress;

    #ifdef __APPLE__
        revery_deregister_icon_progress_cocoa(ip);
    #endif

    CAMLreturn(Val_unit);
}
}