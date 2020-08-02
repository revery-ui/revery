#include "config.h"
#ifdef USE_COCOA
#import "ReveryMenuHandler.h"
#import <Cocoa/Cocoa.h>

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
    NSNumber *nsNumber = self.menuActions[[sender identifier]];
    long ocamlFunc = [nsNumber longValue];
    revery_caml_call(ocamlFunc);
}

-(void)registerOnClick:(NSMenuItem *)menuItem callback:(long)camlCallback {
    NSLog(@"%s", __func__);
    self.menuActions[[menuItem identifier]] =
        [NSNumber numberWithLong:camlCallback];
}

@end

#endif
