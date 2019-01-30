#ifdef __APPLE__
#include <stdio.h>

#import <Cocoa/Cocoa.h>

void revery_alert_cocoa(void *pWin, const char *szMessage) {
    NSWindow* pCocoaWin  = (NSWindow *)pWin;

    NSView *view = [[NSView alloc] init];
    NSAlert *alert = [[NSAlert alloc] init];
    NSString *message = [NSString stringWithUTF8String:szMessage];
    [alert addButtonWithTitle:@"Ok"];
    [alert setMessageText:@"Alert"];
    [alert setInformativeText:message];
    [alert runModal];
}
#endif
