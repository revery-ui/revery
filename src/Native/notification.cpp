#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>


#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

#define Val_none Val_int(0)
static value Val_some(value v) {
    CAMLparam1(v);
    CAMLlocal1(some);
    some = caml_alloc(1, 0);
    Store_field(some, 0, v);
    CAMLreturn(some);
}

#define Some_val(v) Field(v, 0)

extern "C" {
CAMLprim value revery_dispatchNotification(value vNotificationT) {
    CAMLparam1(vNotificationT);

    const char *title;
    const char *body;
    int mute;

    title = String_val(Field(vNotificationT, 0));
    body = String_val(Field(vNotificationT, 1));
    value onClickCaml = Field(vNotificationT, 2);
    mute = Int_val(Field(vNotificationT, 3));
#ifdef __APPLE__
    revery_dispatchNotification_cocoa(title, body, onClickCaml, mute);
#endif

    CAMLreturn(Val_unit);
}
}