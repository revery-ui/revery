#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "caml_values.h"

#define UNUSED(x) (void)(x)

#include "config.h"
#ifdef USE_WIN32
#include "ReveryWin32.h"
#elif USE_COCOA
#include "ReveryCocoa.h"
#elif USE_GTK
#include "ReveryGtk.h"
#endif

CAMLprim value revery_makeTrayHandle(value imagePath_v) {
    CAMLparam1(imagePath_v);

    void *ret;
#ifdef USE_COCOA
    const char *imagePath = String_val(imagePath_v);

    if (imagePath_v == Val_none) {
        ret = NULL;
    } else {
        const char *imagePath = String_val(imagePath_v);
        imagePath = String_val(Some_val(imagePath_v));
        ret = revery_makeTrayHandleImage_cocoa(imagePath);
    }

    UNUSED(imagePath);
#elif USE_WIN32
    /* fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__); */
    ret = NULL;
#else
    /* fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__); */
    ret = NULL;
#endif
    CAMLreturn((value)ret);
}
