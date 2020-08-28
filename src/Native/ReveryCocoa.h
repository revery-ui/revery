void revery_alert_cocoa(void *pWin, const char *szMessage);
char **revery_open_files_cocoa(const char *startDir, const char *fileTypes[],
                               int fileTypesSize, int allowMultiple,
                               int canChooseFiles, int canChooseDirectories,
                               int showHidden, const char* buttonText,
                               const char* title);

/* Notification functions */
void revery_dispatchNotification_cocoa(const char *title, const char *body,
                                       long onClickFunc, int mute);
void revery_scheduleNotificationFromNow_cocoa(const char *title,
        const char *body,
        long onClickFunc, int mute,
        int seconds);
void *revery_getIconHandle_cocoa();

/* Tray */
void revery_setTrayTitle_cocoa(const char *title_v);

/* Icon progress bar functions */
void revery_setIconProgress_cocoa(void* dt, double progress);
void revery_setIconProgressIndeterminate_cocoa(void *dt);
void revery_hideIconProgress_cocoa(void* ip);

/* Image functions */
void *revery_makeImageFromAbsolutePath_cocoa(const char *image_path_v);

/* Open functions */
int revery_openURL_cocoa(const char *url_string);
int revery_openFile_cocoa(const char *path_string);

/* Locale functions */
char *revery_getUserLocale_cocoa();
