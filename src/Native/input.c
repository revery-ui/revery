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

CAMLprim value revery_buttonSetColor(value vButton, value vRed, value vGreen, value vBlue, value vAlpha) {
    CAMLparam5(vButton, vRed, vGreen, vBlue, vAlpha);

    void *button = revery_unwrapPointer(vButton);
    double red = Double_val(vRed);
    double green = Double_val(vGreen);
    double blue = Double_val(vBlue);
    double alpha = Double_val(vAlpha);
#ifdef USE_COCOA
    revery_buttonSetColor_cocoa(button, red, green, blue, alpha);
#else
    UNUSED(button);
    UNUSED(red);
    UNUSED(green);
    UNUSED(blue);
    UNUSED(alpha);
#endif

    CAMLreturn(Val_unit);
}
