#include "config.h"
#ifdef IS_WINDOWS
#include <windows.h>
#include <stdio.h>

void getOperatingSystemVersion_windows(int *major, int *minor, int *build) {
    OSVERSIONINFOW osVersion;
    ZeroMemory(&osVersion, sizeof(OSVERSIONINFOW));
    osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);

    NTSTATUS (WINAPI *rtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation) = NULL;
    HINSTANCE hNtDll = LoadLibrary("ntdll.dll");

    if (hNtDll != NULL) {
        rtlGetVersion = (NTSTATUS (WINAPI *)(PRTL_OSVERSIONINFOW))GetProcAddress(hNtDll, "RtlGetVersion");

        if (rtlGetVersion != NULL) {
            rtlGetVersion(&osVersion);
        }

        FreeLibrary(hNtDll);
    }

    if (rtlGetVersion == NULL) {
        GetVersionEx((OSVERSIONINFO*)&osVersion);
    }

    *major = (int)osVersion.dwMajorVersion;
    *minor = (int)osVersion.dwMinorVersion;
    *build = (int)osVersion.dwBuildNumber;
}

#endif
