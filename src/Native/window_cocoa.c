#include "config.h"
#ifdef USE_COCOA

#import <Cocoa/Cocoa.h>

void revery_windowSetUnsavedWork_cocoa(NSWindow *window, int truth) {
    [window setDocumentEdited:truth];
}

#endif
