#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <stdio.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "config.h"
#ifdef IS_MACOS
#import "ReveryMac.h"
#elif IS_LINUX
#include "ReveryLinux.h"
#elif IS_WINDOWS
#include "ReveryWindows.h"
#endif

CAMLprim value revery_getOperatingSystem() {
    CAMLparam0();
    CAMLlocal1(vOS);
#ifdef IS_ANDROID
    vOS = Val_int(1);
#elif IS_IOS
    vOS = Val_int(2);
#elif IS_LINUX
    int kernel, major, minor, patch;
    getOperatingSystemVersion_linux(&kernel, &major, &minor, &patch);
    vOS = caml_alloc(4, 1);
    Store_field(vOS, 0, Val_int(kernel));
    Store_field(vOS, 1, Val_int(major));
    Store_field(vOS, 2, Val_int(minor));
    Store_field(vOS, 3, Val_int(patch));
#elif IS_WINDOWS
    int major, minor, build;
    getOperatingSystemVersion_windows(&major, &minor, &build);
    vOS = caml_alloc(3, 2);
    Store_field(vOS, 0, Val_int(major));
    Store_field(vOS, 1, Val_int(minor));
    Store_field(vOS, 2, Val_int(build));
#elif IS_MACOS
    int major, minor, bugfix;
    getOperatingSystemVersion_mac(&major, &minor, &bugfix);
    vOS = caml_alloc(3, 0);
    Store_field(vOS, 0, Val_int(major));
    Store_field(vOS, 1, Val_int(minor));
    Store_field(vOS, 2, Val_int(bugfix));
#else
    vOS = Val_int(0);
#endif

    CAMLreturn(vOS);
}
