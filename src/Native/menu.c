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

CAMLprim value revery_getMenuBarHandle() {
    CAMLparam0();
    CAMLlocal1(vMenuBarHandle);

    void *ret;
#ifdef USE_COCOA
    ret = revery_getMenuBarHandle_cocoa();
#else
    ret = NULL;
#endif

    vMenuBarHandle = revery_wrapPointer(ret);
    CAMLreturn(vMenuBarHandle);
}

CAMLprim value revery_createMenu(value vTitle) {
    CAMLparam1(vTitle);
    CAMLlocal1(vMenu);

    const char *title = String_val(vTitle);
    void *ret;
#ifdef USE_COCOA
    ret = revery_createMenu_cocoa(title);
#else
    ret = NULL;
#endif

    vMenu = revery_wrapPointer(ret);
    CAMLreturn(vMenu);
}

CAMLprim value revery_createMenuItem(value vTitle) {
    CAMLparam1(vTitle);
    CAMLlocal1(vMenu);

    const char *title = String_val(vTitle);
    void *ret;
#ifdef USE_COCOA
    ret = revery_createMenuItem_cocoa(title);
#else
    ret = NULL;
#endif

    vMenu = revery_wrapPointer(ret);
    CAMLreturn(vMenu);
}
