#include "config.h"
#ifdef USE_COCOA
#import "ReveryMenuItemTarget.h"
#import <Cocoa/Cocoa.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#import "utilities.h"

static const camlValue *callbackForMenuItem;

@implementation ReveryMenuItemTarget

-(id)init {
    self = [super init];
    if (self) {
        if (callbackForMenuItem == NULL) {
            callbackForMenuItem = caml_named_value("revery_callbackForMenuItem");
        }
    }
    return self;
}

-(void)onMenuItemClick:(NSMenuItem *)sender {
    CAMLparam0();
    CAMLlocal1(vMenuItem);

    // Whether the menu item 'click' was due to a shortcut key
    int fDueToKeyPress = 0;

    switch (NSApp.currentEvent.type) {
    case NSEventTypeKeyDown:
    case NSEventTypeKeyUp:
        fDueToKeyPress = 1;
        break;
    default:
        fDueToKeyPress = 0;
        break;
    }

    if (callbackForMenuItem != NULL) {
        vMenuItem = revery_wrapPointer(sender);
        camlValue args[] = {Val_int(fDueToKeyPress), vMenuItem};
        revery_caml_call_n(*callbackForMenuItem, 2, args);
    } else {
        NSLog(@"Unable to acquire menu item callback!");
    }

    CAMLreturn0;
}

@end
#endif
