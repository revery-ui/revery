extern "C" {
void revery_alert_cocoa(void* pWin, const char* szMessage);
char** revery_open_files_cocoa(const char* startDir, char* fileTypes[],
                               int fileTypesSize, int allowMultiple,
                               int canChooseFiles, int canChooseDirectories,
                               int showHidden, const char* buttonText,
                               const char* title);
}