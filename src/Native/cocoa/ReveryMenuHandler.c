#include "config.h"
#ifdef USE_COCOA
#import "ReveryMenuHandler.h"
#import <Cocoa/Cocoa.h>

#include <pthread.h>

#import "utilities.h"

#define UNUSED(x) (void)(x)

@implementation ReveryMenuHandler
- (id)init {
    self = [super init];
    if (self) {
        _menuActions = [NSMutableDictionary new];
    }

    return self;
}

- (void)menuClickHandler:(id)sender {
    NSValue *nsValue = self.menuActions[[sender identifier]];
    if (nsValue != NULL) {
        long *callback = [nsValue pointerValue];
        if (callback != NULL) {
            revery_caml_call_no_lock(*callback);
        }
    }
}

-(void)registerOnClick:(NSMenuItem *)menuItem callback:(long)camlCallback {
    long *globalCallback = malloc(sizeof(long));
    *globalCallback = camlCallback;
    revery_caml_register_global(globalCallback);
    NSValue *nsValue = [NSValue valueWithPointer:globalCallback];
    self.menuActions[[menuItem identifier]] = nsValue;
}

@end

#endif
