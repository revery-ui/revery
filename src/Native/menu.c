#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#include "caml_values.h"
#include "menu.h"

#include "config.h"
#ifdef USE_WIN32
#include "ReveryWin32.h"
#elif USE_COCOA
#include "ReveryCocoa.h"
#import <Cocoa/Cocoa.h>
#elif USE_GTK
#include "ReveryGtk.h"
#endif
#include "utilities.h"

CAMLprim value revery_getMenuBarHandle() {
    CAMLparam0();
    CAMLlocal1(vMenuBarHandle);

    void *handle;
#ifdef USE_COCOA
    handle = revery_getMenuBarHandle_cocoa();
    [(NSObject *)handle retain];
#else
    handle = NULL;
#endif

    vMenuBarHandle = revery_wrapPointer(handle);
    CAMLreturn(vMenuBarHandle);
}

CAMLprim value revery_menuCreate(value vTitle) {
    CAMLparam1(vTitle);
    CAMLlocal1(vMenu);

    const char *title = String_val(vTitle);
    void *menu;
#ifdef USE_COCOA
    menu = revery_menuCreate_cocoa(title);
    [(NSObject *)menu retain];
#else
    UNUSED(title);
    menu = NULL;
#endif

    vMenu = revery_wrapPointer(menu);
    CAMLreturn(vMenu);
}

void convertCamlKeyEquivalent(value vKeyEquivalent, struct KeyEquivalent *keyEquivalent) {
    keyEquivalent->str = String_val(Field(vKeyEquivalent, 0));
    keyEquivalent->alt = Bool_val(Field(vKeyEquivalent, 1));
    keyEquivalent->shift = Bool_val(Field(vKeyEquivalent, 2));
    keyEquivalent->ctrl = Bool_val(Field(vKeyEquivalent, 3));
}

CAMLprim value revery_menuItemCreate(value vTitle, value vKeyEquivalent) {
    CAMLparam2(vTitle, vKeyEquivalent);
    CAMLlocal1(vMenuItem);
    struct KeyEquivalent keyEquivalent;
    if (vKeyEquivalent != Val_none) {
        convertCamlKeyEquivalent(Some_val(vKeyEquivalent), &keyEquivalent);
    }

    const char *title = String_val(vTitle);
    void *menuItem;
#ifdef USE_COCOA
    menuItem = revery_menuItemCreate_cocoa(title, vKeyEquivalent == Val_none ? NULL : &keyEquivalent);
    [(NSObject *)menuItem retain];
#else
    UNUSED(title);
    menuItem = NULL;
#endif

    vMenuItem = revery_wrapPointer(menuItem);
    CAMLreturn(vMenuItem);
}

CAMLprim value revery_menuNth(value vMenu, value vIdx) {
    CAMLparam2(vMenu, vIdx);
    CAMLlocal1(vMenuItem);

    void *menu = revery_unwrapPointer(vMenu);
    int idx = Int_val(vIdx);
    void *menuItem;
#ifdef USE_COCOA
    menuItem = revery_menuNth_cocoa(menu, idx);
    if (menuItem != NULL) {
        [(NSObject *)menuItem retain];
    }
#else
    UNUSED(menu);
    UNUSED(idx);
    menuItem = NULL;
#endif

    vMenuItem = revery_wrapOptionalPointer(menuItem);
    CAMLreturn(vMenuItem);
}

CAMLprim value revery_menuAddItem(value vMenu, value vMenuItem) {
    CAMLparam2(vMenu, vMenuItem);

    void *menu = revery_unwrapPointer(vMenu);
    void *menuItem = revery_unwrapPointer(vMenuItem);
#ifdef USE_COCOA
    revery_menuAddItem_cocoa(menu, menuItem);
#else
    UNUSED(menu);
    UNUSED(menuItem);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuItemGetSubmenu(value vMenuItem) {
    CAMLparam1(vMenuItem);
    CAMLlocal1(vSubmenu);

    void *menuItem = revery_unwrapPointer(vMenuItem);
    void *submenu;
#ifdef USE_COCOA
    submenu = revery_menuItemGetSubmenu_cocoa(menuItem);
    if (submenu != NULL) {
        [(NSObject *)submenu retain];
    }
#else
    UNUSED(menuItem);
    submenu = NULL;
#endif

    vSubmenu = revery_wrapOptionalPointer(submenu);
    CAMLreturn(vSubmenu);
}

CAMLprim value revery_menuRemoveItem(value vMenu, value vMenuItem) {
    CAMLparam2(vMenu, vMenuItem);

    void *menu = revery_unwrapPointer(vMenu);
    void *menuItem = revery_unwrapPointer(vMenuItem);
#ifdef USE_COCOA
    revery_menuRemoveItem_cocoa(menu, menuItem);
#else
    UNUSED(menu);
    UNUSED(menuItem);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuAddSubmenu(value vParent, value vChild) {
    CAMLparam2(vParent, vChild);

    void *parent = revery_unwrapPointer(vParent);
    void *child = revery_unwrapPointer(vChild);
#ifdef USE_COCOA
    revery_menuAddSubmenu_cocoa(parent, child);
#else
    UNUSED(parent);
    UNUSED(child);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuRemoveSubmenu(value vParent, value vChild) {
    CAMLparam2(vParent, vChild);

    void *parent = revery_unwrapPointer(vParent);
    void *child = revery_unwrapPointer(vChild);
#ifdef USE_COCOA
    revery_menuRemoveSubmenu_cocoa(parent, child);
#else
    UNUSED(parent);
    UNUSED(child);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuInsertItemAt(value vMenu, value vMenuItem, value vIdx) {
    CAMLparam3(vMenu, vMenuItem, vIdx);

    void *menu = revery_unwrapPointer(vMenu);
    void *menuItem = revery_unwrapPointer(vMenuItem);
    int idx = Int_val(vIdx);

#ifdef USE_COCOA
    revery_menuInsertItemAt_cocoa(menu, menuItem, idx);
#else
    UNUSED(menu);
    UNUSED(menuItem);
    UNUSED(idx);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuInsertSubmenuAt(value vParent, value vChild, value vIdx) {
    CAMLparam3(vParent, vChild, vIdx);

    void *parent = revery_unwrapPointer(vParent);
    void *child = revery_unwrapPointer(vChild);
    int idx = Int_val(vIdx);

#ifdef USE_COCOA
    revery_menuInsertSubmenuAt_cocoa(parent, child, idx);
#else
    UNUSED(parent);
    UNUSED(child);
    UNUSED(idx);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuClear(value vMenu) {
    CAMLparam1(vMenu);

    void *menu = revery_unwrapPointer(vMenu);
#ifdef USE_COCOA
    revery_menuClear_cocoa(menu);
#else
    UNUSED(menu);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuDisplayIn(value vMenu, value vWindow, value vX, value vY) {
    CAMLparam4(vMenu, vWindow, vX, vY);

    void *menu = revery_unwrapPointer(vMenu);
    void *window = revery_unwrapPointer(vWindow);
    int x = Int_val(vX);
    int y = Int_val(vY);
#ifdef USE_COCOA
    revery_menuDisplayIn_cocoa(menu, window, x, y);
#else
    UNUSED(menu);
    UNUSED(window);
    UNUSED(x);
    UNUSED(y);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuItemCreateSeparator() {
    CAMLparam0();
    CAMLlocal1(vSeparator);

    void *separator;
#ifdef USE_COCOA
    separator = revery_menuItemCreateSeparator_cocoa();
#else
    separator = NULL;
#endif
    vSeparator = revery_wrapPointer(separator);

    CAMLreturn(vSeparator);
}

CAMLprim value revery_menuItemSetEnabled(value vMenuItem, value vTruth) {
    CAMLparam2(vMenuItem, vTruth);

    void *menuItem = revery_unwrapPointer(vMenuItem);
    int truth = Bool_val(vTruth);

#ifdef USE_COCOA
    revery_menuItemSetEnabled_cocoa(menuItem, truth);
#else
    UNUSED(menuItem);
    UNUSED(truth);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuItemSetVisible(value vMenuItem, value vTruth) {
    CAMLparam2(vMenuItem, vTruth);

    void *menuItem = revery_unwrapPointer(vMenuItem);
    int truth = Bool_val(vTruth);

#ifdef USE_COCOA
    revery_menuItemSetVisible_cocoa(menuItem, truth);
#else
    UNUSED(menuItem);
    UNUSED(truth);
#endif

    CAMLreturn(Val_unit);
}
