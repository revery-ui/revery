#ifdef WIN32

#include <windows.h>

#include <caml/custom.h>
#include <stdio.h>
#include <stdbool.h>

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

static void * g_hook_handle = NULL;

LRESULT CALLBACK WndProc(int msg, WPARAM wParam, LPARAM lParam) {
    MSG m;

    if (msg == HC_ACTION && wParam == PM_REMOVE) {
        m = *(MSG *)lParam;
        if (m.message == WM_COMMAND)
            printf("uid %d called\n", LOWORD(m.wParam));
    }

    return CallNextHookEx((void *) NULL, msg, wParam, lParam);
}

void release_hook(void) {
    /*
    ** we are registered only if we add a hook
    */
    UnhookWindowsHookEx(g_hook_handle);
}

value revery_assign_menu_win32(void *pWindow, value vMenu) {
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

value revery_create_menu_win32(void) {
    value ret = alloc_custom(&menu_ops, sizeof(struct s_menu), 0, 1);

    Menu_val(ret).menu_handle = CreateMenu();

    return ret;
}

value revery_menu_insert_node_string_win32(value vMenu, int position, int uid, const char *pMessage) {
    bool ret = InsertMenu(Menu_val(vMenu).menu_handle, position, MF_BYPOSITION | MF_STRING, uid, pMessage);

    if (!g_hook_handle) {
        g_hook_handle = SetWindowsHookExW(WH_GETMESSAGE, WndProc, (HINSTANCE)NULL, GetCurrentThreadId());

        atexit(release_hook);
    }

    return ret;
}

value revery_menu_delete_node_string_win32(value vMenu, int position) {
    bool ret = RemoveMenu(Menu_val(vMenu).menu_handle, position, MF_BYCOMMAND);

    return ret;
}

value revery_menu_item_configure_instance_label_win32(value vMenu, int position, const char *pMessage) {
    bool ret = ModifyMenuA(Menu_val(vMenu).menu_handle, position, MF_BYCOMMAND, position, pMessage);

    return ret;
}
#endif // WIN32
