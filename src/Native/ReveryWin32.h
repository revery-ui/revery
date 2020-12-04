void revery_alert_win32(void *pWin, const char *szMessage);
void *revery_getIconHandle_win32();
void revery_setIconProgress_win32(void *win, void *ih, float progress);
void revery_setIconProgressIndeterminate_win32(void *win, void *ih);
void revery_hideIconProgress_win32(void *win, void *ih);
int revery_openURL_win32(const char *url_string);
char *revery_getUserLocale_win32();
char **revery_open_files_win32(const char *startDir, int canChooseFiles,
                               int canChooseDirectories, const char *title);
