#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#include "caml_values.h"
#include "menu.h"

#include "config.h"
#ifdef USE_WIN32
#include "ReveryWin32.h"
#elif USE_COCOA
#include "ReveryCocoa.h"
#import <Cocoa/Cocoa.h>
#elif USE_GTK
#include "ReveryGtk.h"
#endif
#include "utilities.h"

CAMLprim value revery_buttonCreate() {
    CAMLparam0();
    CAMLlocal1(vButton);

    void *button;
#ifdef USE_COCOA
    button = revery_buttonCreate_cocoa();
    [(NSObject *)button retain];
#else
    button = NULL;
#endif

    vButton = revery_wrapPointer(button);
    CAMLreturn(vButton);
}
