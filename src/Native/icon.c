#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

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

CAMLprim value revery_getIconHandle() {
    CAMLparam0();
    void *ret;
#ifdef USE_COCOA
    ret = revery_getIconHandle_cocoa();
#elif USE_WIN32
    ret = revery_getIconHandle_win32();
#else
    fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__);
    ret = NULL;
#endif
    value vIconHandle = revery_wrapPointer(ret);
    CAMLreturn(vIconHandle);
}

CAMLprim value revery_setIconProgress(value vWin, value vIconHandle,
                                      value vProgress) {
    CAMLparam3(vWin, vIconHandle, vProgress);
    void *win = (void *)revery_unwrapPointer(vWin);
    void *ih = (void *)revery_unwrapPointer(vIconHandle);
    /* vProgress is an OCaml variant of type Revery_Native.Icon.progress
      It can be either:
        - Indeterminate -- has no type arguments
        - Determinate -- has 1 type argument of float
    */
    // If vProgress "is long", it has no type arguments, and is therefore indeterminate
    if (Is_long(vProgress)) {
#ifdef USE_COCOA
        (void)win;
        revery_setIconProgressIndeterminate_cocoa(ih);
#elif USE_WIN32
        revery_setIconProgressIndeterminate_win32(win, ih);
#else
        fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__);
#endif
    } else if (Is_block(vProgress)) {  // If vProgress "is block", it has a type argument and must be determinate
        float progress = Double_val(Field(vProgress, 0));
#ifdef USE_COCOA
        (void)win;
        revery_setIconProgress_cocoa(ih, progress);
#elif USE_WIN32
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
    void *win = (void *)revery_unwrapPointer(vWin);
    void *ih = (void *)revery_unwrapPointer(vIconHandle);

#ifdef USE_COCOA
    (void)win;
    revery_hideIconProgress_cocoa(ih);
#elif USE_WIN32
    revery_hideIconProgress_win32(win, ih);
#else
    (void)win;
    (void)ih;
#endif

    CAMLreturn(Val_unit);
}
