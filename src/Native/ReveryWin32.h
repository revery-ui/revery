extern "C" {
    void revery_alert_win32(void *pWin, const char* szMessage);
    value revery_create_menu_win32(void);
    value revery_add_string_item_menu_win32(value vMenu, const char * pMessage);
    value revery_assign_menu_win32(void * pWindow, value vMenu);
    value revery_get_application_menu_win32(void * pWindow, value list);
}
