#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>

void *revery_getMenuBarHandle_cocoa() {
    return (void *)[NSApp windowsMenu];
}

void *revery_createMenuItem_cocoa(const char *title) {
    NSString *nsTitle =
        [NSString stringWithCString:title encoding:NSUTF8StringEncoding];

    NSMenuItem *nsMenuItem =
        [[NSMenuItem alloc] initWithTitle:nsTitle action:NULL keyEquivalent:@""];

    return (void *)nsMenuItem;
}

void revery_insertItemIntoMenu_cocoa(void *menu, void *menuItem) {
    NSMenu *nsMenu = (NSMenu *)menu;
    NSMenuItem *nsMenuItem = (NSMenuItem *)menuItem;

    [nsMenu addItem:nsMenuItem];
}

#endif
