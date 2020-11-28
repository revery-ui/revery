#include "config.h"
#ifdef USE_COCOA
#include <stdio.h>

#import <Cocoa/Cocoa.h>
#import "ReveryMenuItemTarget.h"

static ReveryMenuItemTarget *menuItemTarget;

NSMenu *revery_getMenuBarHandle_cocoa() {
    return [[NSApplication sharedApplication] mainMenu];
}

NSMenu *revery_menuCreate_cocoa(const char *title) {
    NSString *nsTitle = [NSString stringWithUTF8String:title];
    return [[NSMenu alloc] initWithTitle:nsTitle];
}

NSMenuItem *revery_menuItemCreate_cocoa(const char *title) {
    if (menuItemTarget == NULL) {
        menuItemTarget = [[ReveryMenuItemTarget alloc] init];
    }

    NSString *nsTitle = [NSString stringWithUTF8String:title];
    NSMenuItem *nsMenuItem =
        [[NSMenuItem alloc] initWithTitle:nsTitle action:@selector(onMenuItemClick:) keyEquivalent:@""];

    [nsMenuItem setTarget:menuItemTarget];

    return nsMenuItem;
}

NSMenuItem *revery_menuNth_cocoa(NSMenu *nsMenu, int idx) {
    if (idx > [nsMenu numberOfItems]) {
        return NULL;
    }

    return [nsMenu itemAtIndex:idx];
}

void revery_menuAddItem_cocoa(NSMenu *nsMenu, NSMenuItem *nsMenuItem) {
    [nsMenu addItem:nsMenuItem];
}

void *revery_menuItemGetSubmenu_cocoa(NSMenuItem *nsMenuItem) {
    if (![nsMenuItem hasSubmenu]) {
        return NULL;
    }

    return [nsMenuItem submenu];
}

void revery_menuAddSubmenu_cocoa(NSMenu *parent, NSMenu *child) {
    NSMenuItem *nsMenuItem =
        [[NSMenuItem alloc] initWithTitle:[child title] action:NULL keyEquivalent:@""];
    [parent addItem:nsMenuItem];
    [parent setSubmenu:child forItem:nsMenuItem];
}

void revery_menuRemoveItem_cocoa(NSMenu *nsMenu, NSMenuItem *nsMenuItem) {
    [nsMenu removeItem:nsMenuItem];
}
#endif
