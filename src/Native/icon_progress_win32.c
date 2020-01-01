#ifdef WIN32

#include <windows.h>
#include <shobjidl.h>
#include <combaseapi.h>
#include <stdio.h>


void *revery_register_icon_progress_win32() {
    CoInitialize(NULL);
    ITaskbarList3 *tbl;
    CoCreateInstance(&CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                     &IID_ITaskbarList3, (void**)&tbl);
    return (void *)tbl;
}

#endif