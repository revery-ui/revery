#include "config.h"
#ifdef USE_COCOA
#import "ReveryButtonTarget.h"
#import <Cocoa/Cocoa.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#import "utilities.h"

static const camlValue *callbackForButton;

@implementation ReveryButtonTarget

-(id)init {
    self = [super init];
    if (self) {
        if (callbackForButton == NULL) {
            callbackForButton = caml_named_value("revery_callbackForButton");
        }
    }
    return self;
}

-(void)onButtonClick:(NSButton *)sender {
    CAMLparam0();
    CAMLlocal1(vButton);

    if (callbackForButton != NULL) {
        vButton = revery_wrapPointer(sender);
        camlValue args[] = {vButton};
        revery_caml_call_n(*callbackForButton, 1, args);
    } else {
        NSLog(@"Unable to acquire button callback!");
    }

    CAMLreturn0;
}

@end
#endif
