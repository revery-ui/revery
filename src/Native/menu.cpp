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
    printf("WARNING - Not implemented: revery_assign_menu_menu");
#endif

    CAMLreturn(ret);
}
}
