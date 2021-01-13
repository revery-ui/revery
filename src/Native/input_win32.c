#include "config.h"
#ifdef USE_WIN32
#include <stdio.h>

#define ISOLATION_AWARE_ENABLED 1
#include <windows.h>
#include <commctrl.h>

static HWND mainWindow;

void inputSetMainWindow_win32(HWND win) {
    mainWindow = win;
}

HWND revery_buttonCreate_win32(const char *title) {
    HWND button = CreateWindow(
        "BUTTON",
        title,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        0,
        0,
        0,
        0,
        mainWindow,
        NULL,
        (HINSTANCE)GetWindowLongPtr(mainWindow, GWLP_HINSTANCE),
        NULL
    );
    return button;
}

#endif
