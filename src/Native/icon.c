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

CAMLprim value revery_getIconHandle() {
    CAMLparam0();
    void *ret;
#ifdef __APPLE__
    ret = revery_getIconHandle_cocoa();
#elif WIN32
    ret = revery_getIconHandle_win32();
#else
    fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__);
    ret = NULL;
#endif
    CAMLreturn((value)ret);
}

CAMLprim value revery_setIconProgress(value vWin, value vIconHandle,
                                      value vProgress) {
    CAMLparam3(vWin, vIconHandle, vProgress);
    void *win = (void *)vWin;
    void *ih = (void *)vIconHandle;
    /* vProgress is an OCaml variant of type Revery_Native.Icon.progress
      It can be either:
        - Indeterminate -- has no type arguments
        - Determinate -- has 1 type argument of float
    */
    // If vProgress "is long", it has no type arguments, and is therefore indeterminate
    if (Is_long(vProgress)) {
#ifdef __APPLE__
        (void)win;
        revery_setIconProgressIndeterminate_cocoa(ih);
#elif WIN32
        revery_setIconProgressIndeterminate_win32(win, ih);
#else
        fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__);
#endif
    } else if (Is_block(vProgress)) {  // If vProgress "is block", it has a type argument and must be determinate
        float progress = Double_val(Field(vProgress, 0));
#ifdef __APPLE__
        (void)win;
        revery_setIconProgress_cocoa(ih, progress);
#elif WIN32
        revery_setIconProgress_win32(win, ih, progress);
#else
        fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__);
        (void)win;
        (void)ih;
        (void)progress;
#endif
    }

    CAMLreturn(Val_unit);
}

CAMLprim value revery_hideIconProgress(value vWin, value vIconHandle) {
    CAMLparam2(vWin, vIconHandle);
    void *win = (void *)vWin;
    void *ih = (void *)vIconHandle;

#ifdef __APPLE__
    (void)win;
    revery_hideIconProgress_cocoa(ih);
#elif WIN32
    revery_hideIconProgress_win32(win, ih);
#else
    (void)win;
    (void)ih;
#endif

    CAMLreturn(Val_unit);
}