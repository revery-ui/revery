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
#elif __APPLE__
    printf("WARNING - Not implemented: revery_create_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_create_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_create_sub_menu");
#endif

    CAMLreturn(ret);
}

CAMLprim value revery_create_popup_menu()
{
    CAMLparam0();
    value ret;

#ifdef WIN32
    /*
    ** Same on Windows
    */
    ret = revery_create_sub_menu_win32();
#elif __APPLE__
    printf("WARNING - Not implemented: revery_create_popup_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_create_popup_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_create_popup_menu");
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
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_string_item_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_string_item_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_string_item_sub_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_add_string_item_popup_menu(value vMenu, value vUid, value vMessage)
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
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_string_item_popup_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_string_item_popup_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_string_item_popup_menu");
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
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_separator_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_separator_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_separator_sub_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_add_separator_popup_menu(value vMenu)
{
    CAMLparam1(vMenu);
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    value ret;

#ifdef WIN32
    /*
    ** Same on Windows
    */
    ret = revery_add_separator_sub_menu_win32(vMenu);
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_separator_popup_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_separator_popup_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_separator_popup_menu");
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
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_sub_menu_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_sub_menu_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_sub_menu_sub_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_add_sub_menu_popup_menu(value vMenu, value vSub, value vMessage)
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
#elif __APPLE__
    printf("WARNING - Not implemented: revery_add_sub_menu_popup_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_add_sub_menu_popup_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_add_sub_menu_popup_menu");
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
#elif __APPLE__
    printf("WARNING - Not implemented: revery_assign_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_assign_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_assign_menu");
#endif

    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_popup_sub_menu(value vWindow, value vMenu, value vX, value vY)
{
    CAMLparam4(vWindow, vMenu, vX, vY);
    void *pWin = (void *)vWindow;
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    int x = Int_val(vX), y = Int_val(vY);
    value ret;

#ifdef WIN32
    ret = revery_popup_sub_menu_win32(pWin, vMenu, x, y);
#elif __APPLE__
    printf("WARNING - Not implemented: revery_popup_sub_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_popup_sub_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_popup_sub_menu");
#endif

/*
** this value is meaningless on Windows :shrug:
*/
    CAMLreturn(Bool_val(ret));
}

CAMLprim value revery_get_application_menu(value vWindow, value vList)
{
    CAMLparam2(vWindow, vList);
    void *pWin = (void *)vWindow;
    value ret;

#ifdef WIN32
    ret = revery_get_application_menu_win32(pWin, vList);
#elif __APPLE__
    printf("WARNING - Not implemented: revery_get_application_menu_cocoa");
#elif __linux__
    printf("WARNING - Not implemented: revery_get_application_menu_gtk");
#else
    printf("WARNING - Not implemented: revery_get_application_menu");
#endif

    CAMLreturn(ret);
}

CAMLprim value revery_refresh_menu_bar(value vWindow)
{
    CAMLparam1(vWindow);
    void *pWin = (void *)vWindow;
    value ret = Val_true; //all is ok

#ifdef WIN32
    ret = Bool_val(revery_refresh_menu_bar_win32(pWin));
#elif __APPLE__
    // I think it is window specific
    //printf("WARNING - Not implemented: revery_refresh_menu_bar_cocoa");
#elif __linux__
    // I think it is window specific
    //printf("WARNING - Not implemented: revery_refresh_menu_bar_gtk");
#else
    printf("WARNING - Not implemented: revery_refresh_menu_bar");
#endif

    CAMLreturn(ret);
}
}
