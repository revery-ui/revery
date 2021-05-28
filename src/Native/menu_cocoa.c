#include "config.h"
#ifdef USE_COCOA
#include <stdio.h>
#include "menu.h"

#import <Cocoa/Cocoa.h>
#import "ReveryMenuItemTarget.h"

static ReveryMenuItemTarget *menuItemTarget;

NSMenu *revery_getMenuBarHandle_cocoa() {
    return [[NSApplication sharedApplication] mainMenu];
}

NSMenu *revery_menuCreate_cocoa(const char *title) {
    NSString *nsTitle = [NSString stringWithUTF8String:title];
    NSMenu *nsMenu = [[NSMenu alloc] initWithTitle:nsTitle];
    [nsMenu setAutoenablesItems:NO];
    return nsMenu;
}

NSMenuItem *revery_menuItemCreate_cocoa(const char *title, struct KeyEquivalent *keyEquivalent) {
    if (menuItemTarget == NULL) {
        menuItemTarget = [[ReveryMenuItemTarget alloc] init];
    }

    NSString *nsTitle = [NSString stringWithUTF8String:title];
    NSMenuItem *nsMenuItem =
        [[NSMenuItem alloc] initWithTitle:nsTitle action:@selector(onMenuItemClick:) keyEquivalent:@""];

    [nsMenuItem setTarget:menuItemTarget];

    if (keyEquivalent != NULL) {
        NSEventModifierFlags modifierFlags = NSCommandKeyMask;
        [nsMenuItem setKeyEquivalent:[NSString stringWithUTF8String:keyEquivalent->str]];
        if (keyEquivalent->alt) {
            modifierFlags |= NSEventModifierFlagOption;
        }
        if (keyEquivalent->shift) {
            modifierFlags |= NSEventModifierFlagShift;
        }
        if (keyEquivalent->ctrl) {
            modifierFlags |= NSEventModifierFlagControl;
        }
        [nsMenuItem setKeyEquivalentModifierMask:modifierFlags];
    }

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

void revery_menuRemoveSubmenu_cocoa(NSMenu *parent, NSMenu *child) {
    NSMutableArray *itemsToRemove = [NSMutableArray array];
    for (NSMenuItem *item in [parent itemArray]) {
        if ([child isEqual:[item submenu]]) {
            [itemsToRemove addObject:item];
        }
    }

    for (NSMenuItem *item in itemsToRemove) {
        [parent removeItem:item];
    }
}

void revery_menuRemoveItem_cocoa(NSMenu *nsMenu, NSMenuItem *nsMenuItem) {
    [nsMenu removeItem:nsMenuItem];
}

void revery_menuInsertItemAt_cocoa(NSMenu *nsMenu, NSMenuItem *nsMenuItem, int idx) {
    [nsMenu insertItem:nsMenuItem atIndex:idx];
}

void revery_menuInsertSubmenuAt_cocoa(NSMenu *parent, NSMenu *child, int idx) {
    NSMenuItem *nsMenuItem =
        [[NSMenuItem alloc] initWithTitle:[child title] action:NULL keyEquivalent:@""];
    [parent insertItem:nsMenuItem atIndex:idx];
    [parent setSubmenu:child forItem:nsMenuItem];
}

void revery_menuClear_cocoa(NSMenu *nsMenu) {
    [nsMenu removeAllItems];
}

void revery_menuDisplayIn_cocoa(NSMenu *nsMenu, NSWindow *nsWindow, int x, int y) {
    NSView *nsView = [nsWindow contentView];
    CGRect frame = [nsView frame];

    float adjY = frame.size.height - (float)y;

    CGPoint point;
    point.x = (float)x;
    point.y = adjY;

    [nsMenu popUpMenuPositioningItem:nil atLocation:point inView:nsView];
}

NSMenuItem *revery_menuItemCreateSeparator_cocoa() {
    return [NSMenuItem separatorItem];
}

void revery_menuItemSetEnabled_cocoa(NSMenuItem *nsMenuItem, int truth) {
    [nsMenuItem setEnabled:truth];
}

void revery_menuItemSetVisible_cocoa(NSMenuItem *nsMenuItem, int truth) {
    [nsMenuItem setHidden:!truth];
}
#endif
