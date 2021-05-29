#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

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

CAMLprim value revery_makeTrayHandle(value imagePath_v) {
    CAMLparam1(imagePath_v);
    CAMLlocal1(result);

#ifdef USE_COCOA
    NSStatusItem *statusItem = [NSStatusBar.systemStatusBar statusItemWithLength:NSVariableStatusItemLength];

    if (imagePath_v != Val_none) {
        const char *imagePath = String_val(Some_val(imagePath_v));

        NSImage *nsImage = revery_makeImageFromAbsolutePath_cocoa(imagePath);

        statusItem.button.image = nsImage;

        UNUSED(imagePath);
    }

    result = revery_wrapPointer(statusItem);

    CAMLreturn(result);
#elif USE_WIN32
    result = caml_alloc(sizeof(NULL), Abstract_tag);
#else
    result = caml_alloc(sizeof(NULL), Abstract_tag);
#endif
    CAMLreturn(result);
}

CAMLprim value revery_setTrayTitle(value trayHandle_v, value title_v) {
    CAMLparam2(trayHandle_v, title_v);
    CAMLlocal1(result);
#ifdef USE_COCOA
    NSStatusItem* statusItem = (NSStatusItem*)(void*)Field(trayHandle_v, 0);

    const char *title = String_val(title_v);

    NSString *nsTitle =
        [NSString stringWithCString:title encoding:NSUTF8StringEncoding];

    statusItem.button.image = NULL;
    statusItem.button.title = nsTitle;

    result = revery_wrapPointer(statusItem);

    CAMLreturn(result);
#elif USE_WIN32
    result = caml_alloc(sizeof(NULL), Abstract_tag);
#else
    result = caml_alloc(sizeof(NULL), Abstract_tag);
#endif
    CAMLreturn(result);
}

void revery_removeTrayItem(value trayHandle_v) {
    CAMLparam1(trayHandle_v);
#ifdef USE_COCOA
    NSStatusItem* statusItem = (NSStatusItem *)revery_unwrapPointer(trayHandle_v);

    [[NSStatusBar systemStatusBar] removeStatusItem: statusItem];

    CAMLreturn0;
#elif USE_WIN32
    CAMLreturn0;
#else
    CAMLreturn0;
#endif
}
