#ifdef WIN32

#include <windows.h>

#include <caml/custom.h>

#include "ReveryWin32.h"
/*
** Define custom operations here
*/

struct s_menu {
    void * menu_handle;
};

#define Menu_val(v) (*((struct s_menu *)Data_custom_val(v)))

int menu_compare(value v1, value v2) {
    return Val_int(Menu_val(v1).menu_handle - Menu_val(v2).menu_handle);
}

/*
** Encapsulation of opaque menu handles (of type s_menu)
** as OCaml custom blocks.
*/
static struct custom_operations menu_ops = {
    "com.outrunlabs.www.revery.menu",
    custom_finalize_default,
    menu_compare,
    custom_hash_default,
    /*
    ** ASK: is it useful to define custom_hash_default
    */
    custom_serialize_default,
    custom_deserialize_default,
    custom_compare_ext_default,

#if OCAML_VERSION < 40800
    /*
    ** ASK: is it useful to define custom_fixed_length_default
    */
    custom_fixed_length_default,
#endif
};

value revery_create_menu_win32(void) {
    value ret = alloc_custom(&menu_ops, sizeof(struct s_menu), 0, 1);

    Menu_val(ret).menu_handle = CreateMenu();

    return ret;
}
#endif // WIN32
