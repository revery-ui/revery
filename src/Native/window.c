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
#import <Cocoa/Cocoa.h>
#elif USE_GTK
#include "ReveryGtk.h"
#endif
#include "utilities.h"

CAMLprim value revery_windowSetUnsavedWork(value vWindow, value vTruth) {
    CAMLparam2(vWindow, vTruth);

    void *window = revery_unwrapPointer(vWindow);
    int truth = Bool_val(vTruth);
#ifdef USE_COCOA
    revery_windowSetUnsavedWork_cocoa(window, truth);
#else
    UNUSED(window);
    UNUSED(truth);
#endif

    CAMLreturn(Val_unit);
}
