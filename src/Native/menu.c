#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "caml_values.h"

#define UNUSED(x) (void)(x)

#include "config.h"
#ifdef USE_WIN32
#include "ReveryWin32.h"
#elif USE_COCOA
#include "ReveryCocoa.h"
#elif USE_GTK
#include "ReveryGtk.h"
#endif

CAMLprim value revery_getMenuBarHandle(value vWindow) {
    CAMLparam1(vWindow);

    void *menuBarHandle;
#ifdef USE_COCOA
    menuBarHandle = revery_getMenuBarHandle_cocoa();
#else
    menuBarHandle = NULL;
#endif
    CAMLreturn((value)menuBarHandle);
}

CAMLprim value revery_createMenuItem(value vTitle) {
    CAMLparam1(vTitle);

    const char *title = String_val(vTitle);

    void *menuItem;
#ifdef USE_COCOA
    menuItem = revery_createMenuItem_cocoa(title);
#else
    menuItem = NULL;
#endif
    CAMLreturn((value)menuItem);
}

CAMLprim value revery_insertItemIntoMenu(value vMenu, value vMenuItem) {
    CAMLparam2(vMenu, vMenuItem);

    void *menu = (void *)vMenu;
    void *menuItem = (void *)vMenuItem;
#ifdef USE_COCOA
    revery_insertItemIntoMenu_cocoa(menu, menuItem);
#else
    UNUSED(menu);
    UNUSED(menuItem);
#endif
    CAMLreturn(Val_unit);
}

CAMLprim value revery_createMenu(value vTitle) {
    CAMLparam1(vTitle);

    const char *title = String_val(vTitle);

    void *menu;
#ifdef USE_COCOA
    menu = revery_createMenu_cocoa(title);
#else
    menu = NULL
#endif
    CAMLreturn((value)menu);
}

CAMLprim value revery_setSubmenuForItem(value vMenuItem, value vMenu) {
    CAMLparam2(vMenuItem, vMenu);

    void *menu = (void *)vMenu;
    void *menuItem = (void *)vMenuItem;
#ifdef USE_COCOA
    revery_setSubmenuForItem_cocoa(menuItem, menu);
#else
    UNUSED(menu);
    UNUSED(menuItem);
#endif
    CAMLreturn(Val_unit);
}

CAMLprim value revery_setOnClickForMenuItem(value vCallback, value vMenuItem) {
    CAMLparam2(vCallback, vMenuItem);

    void *menuItem = (void *)vMenuItem;
#ifdef USE_COCOA
    revery_setOnClickForMenuItem_cocoa(vCallback, menuItem);
#else
    UNUSED(menuItem);
#endif
    CAMLreturn(Val_unit);
}

CAMLprim value revery_displayMenuAtPositionInWindow(value vMenu, value vWindow, value vX, value vY) {
    CAMLparam4(vMenu, vWindow, vX, vY);

    void *menu = (void *)vMenu;
    void *window = (void *)vWindow;
    int x = Int_val(vX);
    int y = Int_val(vY);
#ifdef USE_COCOA
    revery_displayMenuAtPositionInWindow_cocoa(menu, window, x, y);
#else
    UNUSED(menu);
    UNUSED(window);
    UNUSED(x);
    UNUSED(y);
#endif
    CAMLreturn(Val_unit);
}

CAMLprim value revery_removeItemFromMenu(value vMenu, value vMenuItem) {
    CAMLparam2(vMenuItem, vMenu);

    void *menu = (void *)vMenu;
    void *menuItem = (void *)vMenuItem;
#ifdef USE_COCOA
    revery_removeItemFromMenu_cocoa(menu, menuItem);
#else
    UNUSED(menu);
    UNUSED(menuItem);
#endif
    CAMLreturn(Val_unit);
}
