#include <stdio.h>

#import <Cocoa/Cocoa.h>

void revery_alert_cocoa(void *pWin, const char *szMessage) {
    NSWindow* pCocoaWin  = (NSWindow *)pWin;

    NSView *view = [[NSView alloc] init];
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"Ok"];
    [alert setMessageText:@"Alert"];
    [alert setInformativeText:@"Info"];
    [alert runModal];


    printf("ALERT: %s - hwnd: %p\n", szMessage, pCocoaWin);
}
