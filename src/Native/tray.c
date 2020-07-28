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

CAMLprim value revery_makeTrayHandle(value title_v) {
    CAMLparam1(title_v);

    void *ret;
#ifdef USE_COCOA
    const char *title;
    title = String_val(Field(title_v, 1));

    if (Field(title_v, 0) == hash_variant("Text")) {
        ret = revery_makeTrayHandleText_cocoa(title);
    } else {
        ret = revery_makeTrayHandleImage_cocoa(title);
    };

    UNUSED(title);
#elif USE_WIN32
    fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__);
    ret = NULL;
#else
    fprintf(stderr, "WARNING: %s is not implemented on this platform.", __func__);
    ret = NULL;
#endif
    CAMLreturn((value)ret);
}
