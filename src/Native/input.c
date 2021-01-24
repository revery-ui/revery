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

CAMLprim value revery_buttonCreate(value vTitle) {
    CAMLparam1(vTitle);
    CAMLlocal1(vButton);

    const char *title = String_val(vTitle);
    void *button;
#ifdef USE_COCOA
    button = revery_buttonCreate_cocoa(title);
    [(NSObject *)button retain];
#elif USE_WIN32
    button = revery_buttonCreate_win32(title);
#else
    UNUSED(title);
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

CAMLprim value revery_buttonGetDefaultSize(value vButton) {
    CAMLparam1(vButton);
    CAMLlocal1(vSize);

    void *button = revery_unwrapPointer(vButton);
    int width = 0;
    int height = 0;
#ifdef USE_WIN32
    revery_buttonGetDefaultSize_win32(button, &width, &height);
#else
    UNUSED(button);
    UNUSED(width);
    UNUSED(height);
#endif

    vSize = caml_alloc(2, 0);

    Store_field(vSize, 0, Val_int(width));
    Store_field(vSize, 1, Val_int(height));

    CAMLreturn(vSize);
}

CAMLprim value revery_buttonHash(value vButton) {
    CAMLparam1(vButton);
    CAMLlocal1(vHash);

    int hash = 0;
    void *button = revery_unwrapPointer(vButton);
#ifdef USE_WIN32
    hash = revery_buttonHash_win32(button);
#else
    UNUSED(hash);
    UNUSED(button);
#endif

    vHash = Val_int(hash);
    CAMLreturn(vHash);
}
