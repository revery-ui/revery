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
    static HFONT buttonFont = NULL;

    HWND button = CreateWindow(
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

    if (buttonFont == NULL) {
        NONCLIENTMETRICS metrics;
        metrics.cbSize = sizeof(NONCLIENTMETRICS);
        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
        buttonFont = CreateFontIndirect(&metrics.lfCaptionFont);
    }

    SendMessage(button, WM_SETFONT, (LPARAM)buttonFont, 1);
    ShowWindow(button, SW_HIDE);

    return button;
}

void revery_buttonGetDefaultSize_win32(HWND button, int *width, int *height) {
    SIZE size;
    memset(&size, 0, sizeof(SIZE));
    Button_GetIdealSize(
        button,
        &size
    );

    *width = size.cx;
    *height = size.cy;
}

#endif
