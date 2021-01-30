#include "config.h"
#ifdef IS_LINUX
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void getOperatingSystemVersion_linux(int *kernel, int *major, int *minor, int *patch) {
    long ver[16];
    struct utsname unameInfo;
    uname(&unameInfo);

    char *cursor = unameInfo.release;
    int i = 0;
    while (*cursor) {
        if (isdigit(*cursor)) {
            ver[i] = strtol(cursor, &cursor, 10);
            i++;
        } else {
            cursor++;
        }
    }

    *kernel = (int)ver[0];
    *major = (int)ver[1];
    *minor = (int)ver[2];
    *patch = (int)ver[3];
}

#endif
