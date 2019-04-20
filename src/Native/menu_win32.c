#ifdef WIN32

#include <stdio.h>

#include <Windows.h>
#include <winuser.h>

#include <caml/custom.h>

/* Encapsulation of opaque menu handles (of type s_menu)
   as OCaml custom blocks. */

static struct custom_operations menu_ops = {
    "com.outrunlabs.www.revery.menu",
    custom_finalize_default,
    custom_compare_default,
    custom_hash_default,
    custom_serialize_default,
    custom_deserialize_default,
    custom_compare_ext_default
};

struct s_menu
{
    void * menu_handle;
};

#define Menu_val(v) (*((struct s_menu *)Data_custom_val(v)))

value revery_create_menu_win32(void)
{
    value ret = alloc_custom(&menu_ops, sizeof(struct s_menu), 0, 1);

    Menu_val(ret).menu_handle = CreateMenu();

    return ret;
}

value revery_assign_menu_win32(void * pWindow, value vMenu)
{
    return SetMenu(pWindow, Menu_val(vMenu).menu_handle);
}
#endif
