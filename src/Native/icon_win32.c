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
    HBITMAP bmp = (HBITMAP)LoadImage(NULL, "badge.bmp", IMAGE_BITMAP, 0, 0,
                                     LR_LOADFROMFILE);

    char *displayStr;

    if (strlen(badgeStr) <= 3) {
        displayStr = badgeStr;
    } else {
        displayStr = (char *) malloc((6) * sizeof(char));
        displayStr[0] = badgeStr[0];
        displayStr[1] = badgeStr[1];
        displayStr[2] = displayStr[3] = displayStr[4] = '.';
        displayStr[5] = '\0';
    }

    printf("Display string: %s\n", displayStr);

    RECT rect;
    rect.left = 0;
    rect.top = 6;
    rect.bottom = 32;
    rect.right = 32;
    HDC hdc = CreateCompatibleDC(NULL);
    HBITMAP oldBmp = (HBITMAP)SelectObject(hdc, bmp);
    HFONT NewFont = CreateFont(
        20, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        0, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    HBRUSH NewBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, NewFont);
    SelectObject(hdc, NewBrush);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    DrawText(hdc, displayStr, strlen(displayStr), &rect, DT_CENTER | DT_WORDBREAK);
    DeleteObject(NewFont);
    DeleteObject(NewBrush);
    SelectObject(hdc, oldBmp);
    HIMAGELIST imageList = ImageList_Create(32, 32, ILC_COLOR32, 1, 1);
    ImageList_Add(imageList, bmp, NULL);
    HICON hIcon = ImageList_GetIcon(imageList, 0, ILD_IMAGE);
    iconHandle->lpVtbl->SetOverlayIcon(iconHandle, window, hIcon,
                                       L"Badge number");
    ReleaseDC(NULL, hdc);

    if (strlen(badgeStr) > 3) {
        free(displayStr);
    }
}

#endif