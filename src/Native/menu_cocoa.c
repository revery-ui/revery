#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>

void *revery_getMenuBarHandle_cocoa() {
    return (void *)[NSApp mainMenu];
}

#endif
