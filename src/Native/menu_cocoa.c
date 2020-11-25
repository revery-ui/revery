#include "config.h"
#ifdef USE_COCOA
#include <stdio.h>

#import <Cocoa/Cocoa.h>
#import "ReveryMenuItemTarget.h"

static ReveryMenuItemTarget *menuItemTarget;

NSMenu *revery_getMenuBarHandle_cocoa() {
    return [[NSApplication sharedApplication] mainMenu];
}

NSMenu *revery_createMenu_cocoa(const char *title) {
    NSString *nsTitle = [NSString stringWithUTF8String:title];
    return [[NSMenu alloc] initWithTitle:nsTitle];
}

NSMenuItem *revery_createMenuItem_cocoa(const char *title) {
    if (menuItemTarget == NULL) {
        menuItemTarget = [[ReveryMenuItemTarget alloc] init];
    }

    NSString *nsTitle = [NSString stringWithUTF8String:title];
    NSMenuItem *nsMenuItem =
        [[NSMenuItem alloc] initWithTitle:nsTitle action:@selector(onMenuItemClick:) keyEquivalent:@""];

    [nsMenuItem setTarget:menuItemTarget];

    NSMenu *nsMenuBar = revery_getMenuBarHandle_cocoa();
    NSMenu *appMenu = [[nsMenuBar itemAtIndex:0] submenu];

    [appMenu addItem:nsMenuItem];

    return nsMenuItem;
}

#endif
