#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#include "caml_values.h"

#include "config.h"
#ifdef USE_WIN32
#include "ReveryWin32.h"
#elif USE_COCOA
#include "ReveryCocoa.h"
#elif USE_GTK
#include "ReveryGtk.h"
#endif
#include "utilities.h"

CAMLprim value revery_getMenuBarHandle() {
    CAMLparam0();
    CAMLlocal1(vMenuBarHandle);

    void *ret;
#ifdef USE_COCOA
    ret = revery_getMenuBarHandle_cocoa();
#else
    ret = NULL;
#endif

    vMenuBarHandle = revery_wrapPointer(ret);
    CAMLreturn(vMenuBarHandle);
}

CAMLprim value revery_menuCreate(value vTitle) {
    CAMLparam1(vTitle);
    CAMLlocal1(vMenu);

    const char *title = String_val(vTitle);
    void *ret;
#ifdef USE_COCOA
    ret = revery_menuCreate_cocoa(title);
#else
    ret = NULL;
#endif

    vMenu = revery_wrapPointer(ret);
    CAMLreturn(vMenu);
}

CAMLprim value revery_menuItemCreate(value vTitle) {
    CAMLparam1(vTitle);
    CAMLlocal1(vMenu);

    const char *title = String_val(vTitle);
    void *ret;
#ifdef USE_COCOA
    ret = revery_menuItemCreate_cocoa(title);
#else
    ret = NULL;
#endif

    vMenu = revery_wrapPointer(ret);
    CAMLreturn(vMenu);
}

CAMLprim value revery_menuNth(value vMenu, value vIdx) {
    CAMLparam2(vMenu, vIdx);
    CAMLlocal1(vMenuItem);

    void *menu = revery_unwrapPointer(vMenu);
    int idx = Int_val(vIdx);
    void *menuItem;
#ifdef USE_COCOA
    menuItem = revery_menuNth_cocoa(menu, idx);
#else
    menuItem = NULL
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
#else
    submenu = NULL;
#endif

    vSubmenu = revery_wrapOptionalPointer(submenu);
    CAMLreturn(vSubmenu);
}

CAMLprim value revery_menuAddSubmenu(value vParent, value vChild) {
    CAMLparam2(vParent, vChild);

    void *parent = revery_unwrapPointer(vParent);
    void *child = revery_unwrapPointer(vChild);
#ifdef USE_COCOA
    revery_menuAddSubmenu_cocoa(parent, child);
#endif

    CAMLreturn(Val_unit);
}

CAMLprim value revery_menuRemoveItem(value vMenu, value vMenuItem) {
    CAMLparam2(vMenu, vMenuItem);

    void *menu = revery_unwrapPointer(vMenu);
    void *menuItem = revery_unwrapPointer(vMenuItem);
#ifdef USE_COCOA
    revery_menuRemoveItem_cocoa(menu, menuItem);
#endif

    CAMLreturn(Val_unit);
}
