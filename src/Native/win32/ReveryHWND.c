#include "config.h"
#ifdef USE_WIN32
#include <stdio.h>
#include <windows.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "utilities.h"

static WNDPROC defaultWindowProc = NULL;

void callButtonCallback(HWND button) {
    CAMLparam0();
    CAMLlocal1(vButton);
    static const camlValue *callbackForButton = NULL;

    if (callbackForButton == NULL) {
        callbackForButton = caml_named_value("revery_callbackForButton");
    }

    if (callbackForButton == NULL) {
        printf("Unable to acquire button callback!");
    } else {
        vButton = revery_wrapPointer(button);
        camlValue args[] = {vButton};
        revery_caml_call_n(*callbackForButton, 1, args);
    }

    CAMLreturn0;
}


LRESULT CALLBACK buttonWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
    case WM_COMMAND:
        if ((LOWORD(wParam) | ~0xF000) == ~0) {
            HWND button = GetDlgItem(hwnd, LOWORD(wParam));
            callButtonCallback(button);
        }
        break;
    }
    return CallWindowProc(defaultWindowProc, hwnd, uMsg, wParam, lParam);
}

void updateWindowProc_win32(HWND hwnd) {
    defaultWindowProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)buttonWindowProc);
}

CAMLprim value revery_HWND_remove(value vHWND) {
    CAMLparam1(vHWND);
    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);

    /* Unfortunately, controls always have to have parents,
    so, unlike on macOS, we can't "remove" the control
    from it's parent. Rather, we can simply hide it in
    it's current parent */
    ShowWindow(hwnd, SW_HIDE);

    CAMLreturn(Val_unit);
}

CAMLprim value revery_HWND_setFrame(value vHWND, value vX, value vY, value vWidth, value vHeight) {
    CAMLparam5(vHWND, vX, vY, vWidth, vHeight);

    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);
    int x = Int_val(vX);
    int y = Int_val(vY);
    int width = Int_val(vWidth);
    int height = Int_val(vHeight);

    MoveWindow(
        hwnd,
        x,
        y,
        width,
        height,
        TRUE
    );

    CAMLreturn(Val_unit);
}

CAMLprim value revery_HWND_displayIn(value vChild, value vParent) {
    CAMLparam2(vChild, vParent);
    HWND child = (HWND)revery_unwrapPointer(vChild);
    UNUSED(vParent);

    ShowWindow(child, SW_SHOW);

    CAMLreturn(Val_unit);
}

CAMLprim value revery_HWND_toString(value vHWND) {
    CAMLparam1(vHWND);
    CAMLlocal1(vStr);
    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);

    int winTextSize = GetWindowTextLength(hwnd) + 1;

    if (winTextSize > 1) {
        char *windowTitle = malloc(winTextSize * sizeof(char));
        winTextSize = GetWindowText(hwnd, windowTitle, winTextSize);
        windowTitle[winTextSize] = '\0';

        char *str = malloc((winTextSize + 10) * sizeof(char));
        sprintf(str, "HWND: \"%s\"", windowTitle);

        vStr = caml_copy_string(str);

        free(str);
        free(windowTitle);
    } else {
        char str[72];
        sprintf(str, "HWND @ 0x%p", hwnd);
        vStr = caml_copy_string(str);
    }

    CAMLreturn(vStr);
}

#endif
