#include <stdio.h>
#include <math.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "caml_values.h"

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_setBadge(value vBadgeStr) {
    CAMLparam1(vBadgeStr);

    char *badgeStr = String_val(vBadgeStr);

#ifdef __APPLE__
    revery_setBadge_cocoa(badgeStr);
#else
    (void)badgeStr;
#endif

    CAMLreturn(Val_unit);
}

void revery_hideBadge() {
#ifdef __APPLE__
    revery_hideBadge_cocoa();
#endif
}