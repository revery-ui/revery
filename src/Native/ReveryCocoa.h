extern "C" {
   void revery_alert_cocoa(void* pWin, const char* szMessage);
   char** revery_open_files_cocoa(const char* startDir, char* fileTypes[], const char* title);
}
