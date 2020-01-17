#ifdef WIN32

#include <combaseapi.h>
#include <shobjidl.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>

void *revery_getIconHandle_win32() {
    ITaskbarList3 *tbl;
    CoCreateInstance(&CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                     &IID_ITaskbarList3, (void **)&tbl);
    return (void *)tbl;
}

void revery_setIconProgress_win32(void *win, void *ih, float progress) {
    HWND window = (HWND)win;
    ITaskbarList3 *iconHandle = (ITaskbarList3 *)ih;
    iconHandle->lpVtbl->SetProgressState(iconHandle, window, TBPF_NORMAL);
    iconHandle->lpVtbl->SetProgressValue(iconHandle, window, progress * 100,
                                         100);
}

void revery_setIconProgressIndeterminate_win32(void *win, void *ih) {
    HWND window = (HWND)win;
    ITaskbarList3 *iconHandle = (ITaskbarList3 *)ih;
    iconHandle->lpVtbl->SetProgressState(iconHandle, window,
                                         TBPF_INDETERMINATE);
}

void revery_hideIconProgress_win32(void *win, void *ih) {
    HWND window = (HWND)win;
    ITaskbarList3 *iconHandle = (ITaskbarList3 *)ih;
    iconHandle->lpVtbl->SetProgressState(iconHandle, window, TBPF_NOPROGRESS);
}

void revery_setIconBadge_win32(void *win, void *ih, char *badgeStr) {
    HWND window = (HWND)win;
    ITaskbarList3 *iconHandle = (ITaskbarList3 *)ih;

    HDC hDC = GetDC(NULL);

    // Load the badge template into memory
    HBITMAP bmp = CreateCompatibleBitmap(hDC, 32, 32);

    RECT colorRect;
    colorRect.left = colorRect.top = 0;
    colorRect.right = 32;
    colorRect.bottom = 32;
    HBRUSH hBrush = CreateSolidBrush(RGB(238, 17, 17));


    char *displayStr;

    // Display the full string if it's 3 chars or less
    if (strlen(badgeStr) <= 3) {
        displayStr = badgeStr;
    } else {
        // Malloc a string and add the first two characters...
        displayStr = (char *)malloc((6) * sizeof(char));
        displayStr[0] = badgeStr[0];
        displayStr[1] = badgeStr[1];
        // ...an ellipsis...
        displayStr[2] = displayStr[3] = displayStr[4] = '.';
        // ...and a null termination.
        displayStr[5] = '\0';
    }


    // Create a rect to paint the text into on top of the bitmap
    RECT textRect;
    textRect.left = 0;
    // Center the text in the rect
    textRect.top = 6;
    textRect.bottom = 32;
    textRect.right = 32;


    HDC memHDC = CreateCompatibleDC(hDC);
    // Load the bitmap, and save the old one so we don't leak it
    HBITMAP oldBmp = (HBITMAP)SelectObject(memHDC, bmp);
    FillRect(memHDC, &colorRect, hBrush);
    DeleteObject(hBrush);

    HFONT hFont = CreateFont(
                      20, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                      0, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    SelectObject(memHDC, hFont);
    // Make sure the text background is transparent
    SetBkMode(memHDC, TRANSPARENT);
    SetTextColor(memHDC, RGB(255, 255, 255));
    DrawText(memHDC, displayStr, strlen(displayStr), &textRect, DT_CENTER | DT_WORDBREAK);
    // Delete the font and the old bitmap
    DeleteObject(hFont);
    SelectObject(memHDC, oldBmp);

    // This is to convert the HBITMAP into an HICON
    HIMAGELIST imageList = ImageList_Create(32, 32, ILC_COLOR32, 1, 1);
    ImageList_Add(imageList, bmp, NULL);
    HICON hIcon = ImageList_GetIcon(imageList, 0, ILD_IMAGE);

    // Set the icon
    iconHandle->lpVtbl->SetOverlayIcon(iconHandle, window, hIcon,
                                       L"Badge number");

    DeleteObject(imageList);

    // Delete the DC
    DeleteDC(memHDC);

    if (strlen(badgeStr) > 3) {
        free(displayStr);
    }
}

void revery_hideIconBadge_win32(void *win, void *ih) {
    HWND window = (HWND)win;
    ITaskbarList3 *iconHandle = (ITaskbarList3 *)ih;
    iconHandle->lpVtbl->SetOverlayIcon(iconHandle, window, NULL, NULL);
}

#endif