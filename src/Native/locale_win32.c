#ifdef _WIN32

#include "win32_target.h"

#include <windows.h>
#include <stdlib.h>

char *revery_getUserLocale_win32() {
    WCHAR locale[LOCALE_NAME_MAX_LENGTH];
    char *localeStr = malloc(LOCALE_NAME_MAX_LENGTH * sizeof(char));

    GetUserDefaultLocaleName(locale, LOCALE_NAME_MAX_LENGTH);

    wcstombs(localeStr, locale, sizeof(localeStr));
    return localeStr;
}

#endif
