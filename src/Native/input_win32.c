#include "config.h"
#ifdef USE_WIN32

#define _WIN32_IE 0x0600
#define ISOLATION_AWARE_ENABLED 1
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>

static HWND mainWindow;

void inputSetMainWindow_win32(HWND hwnd) {
    mainWindow = hwnd;
}

HWND revery_buttonCreate_win32(const char *title) {
    HWND button = CreateWindowEx(
                      BS_PUSHBUTTON,
                      "BUTTON",
                      title,
                      WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
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

void revery_buttonGetDefaultSize_win32(HWND button, int *width, int *height) {
    SIZE size;
    memset(&size, 0, sizeof(SIZE));
    Button_GetIdealSize(
        button,
        &size
    );

    printf("%s: %ld, %ld\n", __func__, size.cx, size.cy);

    *width = size.cx;
    *height = size.cy;
}

#endif
