#include "config.h"
#ifdef USE_WIN32

#include <windows.h>
#include <shellapi.h>
#include <stdint.h>

int revery_openURL_win32(const char *url_string) {
    /* We have to do this pointer magic because HINSTANCE is a
    legacy type that is meant to be converted.
    See https://docs.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutea#return-value
    */
    return (int)(intptr_t)ShellExecute(
               NULL,
               "open",
               url_string,
               NULL,
               NULL,
               SW_SHOW
           );
}

#endif
