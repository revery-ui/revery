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
