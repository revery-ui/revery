extern "C" {
    void revery_alert_win32(void *pWin, const char* szMessage);
    value revery_create_menu_win32(void);
    value revery_create_sub_menu_win32(void);
    value revery_add_string_item_menu_win32(value vMenu, int uid, const char * pMessage);
    value revery_add_separator_sub_menu_win32(value vMenu);
    value revery_add_sub_menu_win32(value vMenu, value vSub, const char * pMessage);
    value revery_assign_menu_win32(void * pWindow, value vMenu);
    value revery_popup_sub_menu_win32(void * pWindow, value vMenu, int x, int y);
    value revery_get_application_menu_win32(void * pWindow, value list);
}
