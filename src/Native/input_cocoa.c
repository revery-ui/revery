#include "config.h"
#ifdef USE_COCOA
#include <stdio.h>
#include "menu.h"

#import <Cocoa/Cocoa.h>
#import "ReveryButtonTarget.h"
#import "ReveryNSViewCoords.h"

static ReveryButtonTarget *buttonTarget;

NSButton *revery_buttonCreate_cocoa(const char *title) {
    if (buttonTarget == NULL) {
        buttonTarget = [[ReveryButtonTarget alloc] init];
    }

    NSString *nsTitle = [NSString stringWithUTF8String:title];
    NSButton *nsButton =
        [NSButton buttonWithTitle:nsTitle target:buttonTarget action:@selector(onButtonClick:)];

    [nsButton setReveryX:0.0];
    [nsButton setReveryY:0.0];

    return nsButton;
}

#endif
