#ifdef WIN32

#include <stdio.h>
#include <stdbool.h>

#include <Windows.h>
#include <winuser.h>

#include <caml/alloc.h>
#include <caml/callback.h>
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
static value * g_menu_dispatch = NULL;
static void * g_hook_handle = NULL;

LRESULT CALLBACK WndProc(int msg, WPARAM wParam, LPARAM lParam)
{
    MSG m;

    if (msg == HC_ACTION && wParam == PM_REMOVE)
    {
        m = *(MSG *)lParam;
        if (m.message == WM_COMMAND)
        {
            printf("%d is clicked\n", LOWORD(m.wParam));

            /* First time around, look up by name */
            if (g_menu_dispatch == NULL)
                g_menu_dispatch = caml_named_value("menu_dispatch");
            printf("menu_dispatch: %p, nth: %d\n", g_menu_dispatch, LOWORD(m.wParam));
            caml_callback(*g_menu_dispatch, Val_int(LOWORD(m.wParam)));
        }
    }

    return CallNextHookEx((void *) NULL, msg, wParam, lParam);
}

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
    if (!g_hook_handle)
        g_hook_handle = SetWindowsHookExW(WH_GETMESSAGE, WndProc, (HINSTANCE)NULL, GetCurrentThreadId());

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
