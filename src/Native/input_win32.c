#include "config.h"
#ifdef USE_WIN32

#define _WIN32_IE 0x0600
#define ISOLATION_AWARE_ENABLED 1
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>

/* IDs usually start at 100, but we'll start at 0xF000.
 This makes it easier to determine later if one of the
 buttons that was clicked was made by us by doing some
 bitmasking (see ReveryHWND.c) */
static size_t currentID = 0xF000;
static HWND mainWindow;

void inputSetMainWindow_win32(HWND hwnd) {
    mainWindow = hwnd;
}

int revery_buttonHash_win32(HWND hwnd) {
    return GetDlgCtrlID(hwnd);
}

HWND revery_buttonCreate_win32(const char *title) {
    static HFONT buttonFont = NULL;

    HWND button = CreateWindow(
                      "BUTTON",
                      title,
                      WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                      0,
                      0,
                      0,
                      0,
                      mainWindow,
                      (HMENU)currentID++,
                      (HINSTANCE)GetWindowLongPtr(mainWindow, GWLP_HINSTANCE),
                      NULL
                  );

    // If we haven't loaded in the default font yet, do so now
    if (buttonFont == NULL) {
        NONCLIENTMETRICS metrics;
        metrics.cbSize = sizeof(NONCLIENTMETRICS);
        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
        // The Win32 documentation suggests using the caption font for buttons
        buttonFont = CreateFontIndirect(&metrics.lfCaptionFont);
    }

    SendMessage(button, WM_SETFONT, (LPARAM)buttonFont, 1);
    ShowWindow(button, SW_HIDE);

    return button;
}

void revery_buttonGetDefaultSize_win32(HWND button, int *width, int *height) {
    SIZE size;
    memset(&size, 0, sizeof(SIZE));
    // This only works with COMCTRL.DLL >= v6.0, so it might not work pre-XP
    // If someone wants to use native buttons on < WinXP, they'll have to
    // manually specify the width and height
    Button_GetIdealSize(
        button,
        &size
    );

    *width = size.cx;
    *height = size.cy;
}

#endif
