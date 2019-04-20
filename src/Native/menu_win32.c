#ifdef WIN32

#include <stdio.h>
#include <stdbool.h>

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

static int g_unique_identifier = 0;

value revery_create_menu_win32(void)
{
    value ret = alloc_custom(&menu_ops, sizeof(struct s_menu), 0, 1);

    Menu_val(ret).menu_handle = CreateMenu();

    return ret;
}

value revery_add_string_item_menu_win32(value vMenu, const char * pMessage)
{
    bool ret = AppendMenu(Menu_val(vMenu).menu_handle, MF_STRING, g_unique_identifier, pMessage);

    g_unique_identifier++;

    return ret;
}

value revery_assign_menu_win32(void * pWindow, value vMenu)
{
/*
** TODO: apply this remarks
** Remarks:
**
** The window is redrawn to reflect the menu change.
** A menu can be assigned to any window that is not a child window.
**
** The SetMenu function replaces the previous menu, if any,
** but it does not destroy it. An application should call the DestroyMenu
** function to accomplish this task.
*/
    return SetMenu(pWindow, Menu_val(vMenu).menu_handle);
}
#endif
