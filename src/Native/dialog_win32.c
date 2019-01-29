#ifdef WIN32

#include <stdio.h>

#include <Windows.h>
#include <winuser.h>

void
revery_alert_win32(void *pWin, const char *szMessage) {
    HWND hwnd = (HWND)pWin;

    int msgboxId = MessageBox(
            hwnd,
            szMessage,
            "Alert",
            MB_ICONWARNING | MB_OK
            );
}
#endif
