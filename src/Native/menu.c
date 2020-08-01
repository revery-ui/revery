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

CAMLprim value revery_getMenuBarHandle(value vWindow) {
    CAMLparam1(vWindow);

    void *menuBarHandle;
#ifdef USE_COCOA
    menuBarHandle = revery_getMenuBarHandle_cocoa();
#else
#endif
    CAMLreturn((value)menuBarHandle);
}
