void revery_alert_win32(void *pWin, const char* szMessage);
void* revery_getIconHandle_win32();
void revery_setIconProgress_win32(void *win, void *ih, float progress);
void revery_setIconProgressIndeterminate_win32(void *win, void *ih);
void revery_hideIconProgress_win32(void *win, void *ih);
void revery_setIconBadge_win32(void *win, void *ih, char *badgeStr);
void revery_hideIconBadge_win32(void *win, void *ih);