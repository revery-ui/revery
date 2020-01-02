
void revery_alert_cocoa(void *pWin, const char *szMessage);
char **revery_open_files_cocoa(const char *startDir, char *fileTypes[],
                               int fileTypesSize, int allowMultiple,
                               int canChooseFiles, int canChooseDirectories,
                               int showHidden, const char* buttonText,
                               const char* title);
void revery_dispatchNotification_cocoa(const char *title, const char *body,
                                       long onClickFunc, int mute);
void revery_scheduleNotificationFromNow_cocoa(const char *title,
        const char *body,
        long onClickFunc, int mute,
        int seconds);
void* revery_getIconHandle_cocoa();
void revery_setIconProgress_cocoa(void* dt, double progress);
void revery_setIconProgressIndeterminate_cocoa(void *dt);
void revery_hideIconProgress_cocoa(void* ip);