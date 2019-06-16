#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

extern "C" {
CAMLprim value revery_menuSupported() {
#ifdef WIN32
  return Val_true;
#elif __APPLE__
  return Val_false;
#elif __linux__
  return Val_false;
#else
  return Val_false;
#endif
}

CAMLprim value revery_create_menu()
{
    CAMLparam0();
    value ret;

#ifdef WIN32
    ret = revery_create_menu_win32();
    printf("We have created menu (%p)\n", ret);
#elif __APPLE__
    printf("WARNING - Not implemented: revery_create_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_create_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_create_menu");
#endif

    CAMLreturn(ret);
}

CAMLprim value revery_create_sub_menu()
{
    CAMLparam0();
    value ret;

#ifdef WIN32
    ret = revery_create_sub_menu_win32();
    printf("We have created submenu (%p)\n", ret);
#elif __APPLE__
    printf("WARNING - Not implemented: revery_create_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_create_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_create_sub_menu");
#endif

    CAMLreturn(ret);
}

CAMLprim value revery_add_string_item_menu(value vMenu, value vUid, value vMessage)
{
    CAMLparam3(vMenu, vUid, vMessage);
    const char * pMessage = String_val(vMessage);
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    int uid = Int_val(vUid);
    value ret;

#ifdef WIN32
    ret = revery_add_string_item_menu_win32(vMenu, uid, pMessage);
    printf("We have add string (%s) to menu (%p): %s\n", pMessage, vMenu, ret ? "true" : "false");
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_string_item_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_string_item_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_string_item_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_add_string_item_sub_menu(value vMenu, value vUid, value vMessage)
{
    CAMLparam3(vMenu, vUid, vMessage);
    const char * pMessage = String_val(vMessage);
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    int uid = Int_val(vUid);
    value ret;

#ifdef WIN32
    /*
    ** Same on windows
    */
    ret = revery_add_string_item_menu_win32(vMenu, uid, pMessage);
    printf("We have add string (%s) to submenu (%p): %s\n", pMessage, vMenu, ret ? "true" : "false");
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_string_item_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_string_item_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_string_item_sub_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_add_separator_sub_menu(value vMenu)
{
    CAMLparam1(vMenu);
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    value ret;

#ifdef WIN32
    ret = revery_add_separator_sub_menu_win32(vMenu);
    printf("We have add separator to submenu (%p): %s\n", vMenu, ret ? "true" : "false");
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_separator_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_separator_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_separator_sub_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_add_sub_menu(value vMenu, value vSub, value vMessage)
{
    CAMLparam3(vMenu, vSub, vMessage);
    const char * pMessage = String_val(vMessage);
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    value ret;

#ifdef WIN32
    ret = revery_add_sub_menu_win32(vMenu, vSub, pMessage);
    printf("We have add submenu (%s) to menu (%p): %s\n", pMessage, vMenu, ret ? "true" : "false");
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_sub_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_add_sub_menu_sub_menu(value vMenu, value vSub, value vMessage)
{
    CAMLparam3(vMenu, vSub, vMessage);
    const char * pMessage = String_val(vMessage);
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    value ret;

#ifdef WIN32
    /*
    ** Same on windows
    */
    ret = revery_add_sub_menu_win32(vMenu, vSub, pMessage);
    printf("We have add submenu (%s) to submenu (%p): %s\n", pMessage, vMenu, ret ? "true" : "false");
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_sub_menu_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_sub_menu_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_sub_menu_sub_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_assign_menu(value vWindow, value vMenu)
{
    CAMLparam2(vWindow, vMenu);
    void *pWin = (void *)vWindow;
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    value ret;

#ifdef WIN32
    ret = revery_assign_menu_win32(pWin, vMenu);
    printf("We have assign menu (%p) to window (%p): %s\n", vMenu, pWin, ret ? "true" : "false");
#elif __APPLE__
    printf("WARNING - Not implemented: revery_assign_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_assign_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_assign_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_get_application_menu(value vWindow, value vList)
{
    CAMLparam2(vWindow, vList);
    void *pWin = (void *)vWindow;
    value ret;

#ifdef WIN32
    ret = revery_get_application_menu_win32(pWin, vList);
    printf("We get the application menu (%p)\n", ret);
#elif __APPLE__
    printf("WARNING - Not implemented: revery_get_application_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_get_application_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_get_application_menu");
#endif

    CAMLreturn(ret);
}
}
