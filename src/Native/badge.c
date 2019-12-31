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

CAMLprim value revery_setBadgeNumber(value vBadgeNum) {
    CAMLparam1(vBadgeNum);

    int badgeNum = Int_val(vBadgeNum);
    char badgeNumStr[(int)((ceil(log10(badgeNum)) + 1) * sizeof(char))];
    sprintf(badgeNumStr, "%d", badgeNum);

#ifdef __APPLE__
    revery_setBadgeNumber_cocoa(badgeNumStr);
#else
    (void)badgeNumStr;
#endif

    CAMLreturn(Val_unit);
}

void revery_hideBadge() {
#ifdef __APPLE__
    revery_hideBadge_cocoa();
#endif
}