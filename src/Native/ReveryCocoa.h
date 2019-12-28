
void revery_alert_cocoa(void *pWin, const char *szMessage);
char **revery_open_files_cocoa(const char *startDir, char *fileTypes[],
                               int fileTypesSize, int allowMultiple,
                               int canChooseFiles, int canChooseDirectories,
                               int showHidden, const char *buttonText,
                               const char *title);
void revery_dispatchNotification_cocoa(const char *title, const char *body,
                                       long onClickFunc, int mute);
void revery_scheduleNotificationFromNow_cocoa(const char *title,
                                              const char *body,
                                              long onClickFunc, int mute,
                                              int seconds);
void revery_setBadgeNumber_cocoa(char *badgeNumStr);
void revery_hideBadge_cocoa();