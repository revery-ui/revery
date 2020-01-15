#ifdef WIN32

#include <windows.h>
#include <shobjidl.h>
#include <combaseapi.h>
#include <stdio.h>

void *revery_getIconHandle_win32() {
    ITaskbarList3 *tbl;
    CoCreateInstance(&CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                     &IID_ITaskbarList3, (void**)&tbl);
    return (void *)tbl;
}

void revery_setIconProgress_win32(void *win, void *ih, float progress) {
    HWND window = (HWND)win;
    ITaskbarList3 *iconHandle = (ITaskbarList3 *)ih;
    iconHandle->lpVtbl->SetProgressState(iconHandle, window, TBPF_NORMAL);
    iconHandle->lpVtbl->SetProgressValue(iconHandle, window, progress * 100, 100);
}

void revery_setIconProgressIndeterminate_win32(void *win, void *ih) {
    HWND window = (HWND)win;
    ITaskbarList3 *iconHandle = (ITaskbarList3 *)ih;
    iconHandle->lpVtbl->SetProgressState(iconHandle, window, TBPF_INDETERMINATE);
}

void revery_hideIconProgress_win32(void *win, void *ih) {
    HWND window = (HWND)win;
    ITaskbarList3 *iconHandle = (ITaskbarList3 *)ih;
    iconHandle->lpVtbl->SetProgressState(iconHandle, window, TBPF_NOPROGRESS);
}

#endif