#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>

#include <Windows.h>
#include <winuser.h>

CAMLprim value
revery_alertSupported() {
    return Val_true;
}

CAMLprim value
revery_alert(value vWindow, value vMessage) {
    CAMLparam2(vWindow, vMessage);

    const char *szMessage = String_val(vMessage);
    HWND hwnd = (HWND)vWindow;

    int msgboxId = MessageBox(
            hwnd,
            szMessage,
            "Alert",
            MB_ICONWARNING | MB_OK
            );

    printf("ALERT: %s - hwnd: %p\n", szMessage, hwnd);
    return Val_unit;
}
