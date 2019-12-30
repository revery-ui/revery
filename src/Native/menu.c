#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_menuSupported() {
#ifdef WIN32
    return Val_false;
#elif __APPLE__
    return Val_false;
#elif __linux__
    return Val_false;
#else
    return Val_false;
#endif
}

value revery_create_menu() {
    CAMLparam0();
    value ret = 0;

#ifdef WIN32
    ret = revery_create_menu_win32();
#elif __APPLE__
    fprintf(stderr, "WARNING - Not implemented: %s_cocoa", __func__);
#elif __linux__
    fprintf(stderr, "WARNING - Not implemented: %s_gtk", __func__);
#else
    fprintf(stderr, "WARNING - Not implemented: %s", __func__);
#endif

    CAMLreturn(ret);
}

value revery_menu_insert_node_string(value vMenu, value vPosition, value vUid, value vMessage) {
    CAMLparam4(vMenu, vPosition, vUid, vMessage);
    const char * pMessage = String_val(vMessage);
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    int uid = Int_val(vUid);
    int position = Int_val(vPosition);
    value ret = 0;

#ifdef WIN32
    ret = revery_menu_insert_node_string_win32(vMenu, position, uid, pMessage);
#elif __APPLE__
    fprintf(stderr, "WARNING - Not implemented: %s_cocoa", __func__);
    (void)pMessage;
    (void)uid;
    (void)position;
#elif __linux__
    fprintf(stderr, "WARNING - Not implemented: %s_gtk", __func__);
    (void)pMessage;
    (void)uid;
    (void)position;
#else
    fprintf(stderr, "WARNING - Not implemented: %s", __func__);
    (void)pMessage;
    (void)uid;
    (void)position;
#endif

    CAMLreturn(Bool_val(ret));
}

value revery_menu_delete_node_string(value vMenu, value vPosition) {
    CAMLparam2(vMenu, vPosition);
    /*
    ** void *pMenu = (void *)vMenu;
    ** it is a custom type
    */
    int position = Int_val(vPosition);
    value ret = 0;

#ifdef WIN32
    ret = revery_menu_delete_node_string_win32(vMenu, position);
#elif __APPLE__
    fprintf(stderr, "WARNING - Not implemented: %s_cocoa", __func__);
    (void)position;
#elif __linux__
    fprintf(stderr, "WARNING - Not implemented: %s_gtk", __func__);
    (void)position;
#else
    fprintf(stderr, "WARNING - Not implemented: %s", __func__);
    (void)position;
#endif

    CAMLreturn(Bool_val(ret));
}
