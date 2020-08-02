#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>
#import "ReveryMenuHandler.h"

ReveryMenuHandler *revery_menuHandler = NULL;

void *revery_getMenuBarHandle_cocoa() {
    return (void *)[NSApp mainMenu];
}

void *revery_createMenuItem_cocoa(const char *title) {
    if (revery_menuHandler == NULL) {
        revery_menuHandler = [[ReveryMenuHandler alloc] init];
    }

    NSString *nsTitle =
        [NSString stringWithCString:title encoding:NSUTF8StringEncoding];

    NSMenuItem *nsMenuItem =
        [[NSMenuItem alloc] initWithTitle:nsTitle action:@selector(menuClickHandler:) keyEquivalent:@""];

    [nsMenuItem setTarget:revery_menuHandler];

    return (void *)nsMenuItem;
}

void revery_insertItemIntoMenu_cocoa(void *menu, void *menuItem) {
    NSMenu *nsMenu = (NSMenu *)menu;
    NSMenuItem *nsMenuItem = (NSMenuItem *)menuItem;

    [nsMenu addItem:nsMenuItem];
}

void *revery_createMenu_cocoa(const char *title) {
    NSString *nsTitle =
        [NSString stringWithCString:title encoding:NSUTF8StringEncoding];

    NSMenu *nsMenu =
        [[NSMenu alloc] initWithTitle:nsTitle];

    return (void *)nsMenu;
}

void revery_setSubmenuForItem_cocoa(void *menuItem, void *menu) {
    NSMenu *nsMenu = (NSMenu *)menu;
    NSMenuItem *nsMenuItem = (NSMenuItem *)menuItem;

    [nsMenuItem setSubmenu:nsMenu];
}

void revery_setOnClickForMenuItem_cocoa(void *menuItem, long camlCallback) {
    if (revery_menuHandler != NULL) {
        NSMenuItem *nsMenuItem = (NSMenuItem *)menuItem;
        [revery_menuHandler registerOnClick:nsMenuItem callback:camlCallback];
    }
}
#endif
