#include "config.h"
#ifdef USE_WIN32
#include <stdio.h>

#define ISOLATION_AWARE_ENABLED 1
#include <windows.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "utilities.h"

#define WIDTH(rect) ((rect).right - (rect).left)
#define HEIGHT(rect) ((rect).bottom - (rect).top)

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

CAMLprim value revery_HWND_setAll(value vHWND, value vX, value vY, value vWidth, value vHeight) {
    CAMLparam5(vHWND, vX, vY, vWidth, vHeight);
    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);
    // int x = Int_val(vX);

    // RECT rect;
    // GetClientRect(
    //     hwnd,
    //     &rect
    // );

    // int width = WIDTH(rect);
    // int height = HEIGHT(rect);
    // int y = rect.top;

    MoveWindow(
        hwnd,
        Int_val(vX),
        Int_val(vY),
        Int_val(vWidth),
        Int_val(vHeight),
        TRUE
    );

    CAMLreturn(Val_unit);
}

CAMLprim value revery_HWND_setX(value vHWND, value vX) {
    CAMLparam2(vHWND, vX);
    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);
    int x = Int_val(vX);

    RECT rect;
    GetClientRect(
        hwnd,
        &rect
    );

    int width = WIDTH(rect);
    int height = HEIGHT(rect);
    int y = rect.top;

    BOOL success = MoveWindow(
        hwnd,
        x,
        y,
        width,
        height,
        TRUE
    );
    UNUSED(hwnd);
    UNUSED(x);
    UNUSED(y);
    UNUSED(width);
    UNUSED(height);

    GetClientRect(
        hwnd,
        &rect
    );
    UNUSED(success);

    CAMLreturn(Val_unit);
}

CAMLprim value revery_HWND_setY(value vHWND, value vY) {
    CAMLparam2(vHWND, vY);
    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);
    int y = Int_val(vY);

    RECT rect;
    GetClientRect(
        hwnd,
        &rect
    );

    int width = WIDTH(rect);
    int height = HEIGHT(rect);
    int x = rect.left;

    UNUSED(hwnd);
    UNUSED(x);
    UNUSED(y);
    UNUSED(width);
    UNUSED(height);
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

CAMLprim value revery_HWND_getX(value vHWND) {
    CAMLparam1(vHWND);
    CAMLlocal1(vX);

    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);

    RECT rect;
    GetClientRect(
        hwnd,
        &rect
    );

    vX = Val_int(rect.left);

    CAMLreturn(vX);
}

CAMLprim value revery_HWND_getY(value vHWND) {
    CAMLparam1(vHWND);
    CAMLlocal1(vY);

    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);

    RECT rect;
    GetClientRect(
        hwnd,
        &rect
    );

    vY = Val_int(rect.top);

    CAMLreturn(vY);
}

CAMLprim value revery_HWND_setWidth(value vHWND, value vWidth) {
    CAMLparam2(vHWND, vWidth);
    
    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);
    int width = Int_val(vWidth);

    RECT rect;
    GetClientRect(
        hwnd,
        &rect
    );
    int x = rect.left;
    int y = rect.top;
    int height = HEIGHT(rect);
    UNUSED(hwnd);
    UNUSED(x);
    UNUSED(y);
    UNUSED(width);
    UNUSED(height);

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

CAMLprim value revery_HWND_setHeight(value vHWND, value vHeight) {
    CAMLparam2(vHWND, vHeight);
    
    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);
    int height = Int_val(vHeight);

    RECT rect;
    GetClientRect(
        hwnd,
        &rect
    );
    int x = rect.left;
    int y = rect.top;
    int width = WIDTH(rect);

    UNUSED(hwnd);
    UNUSED(x);
    UNUSED(y);
    UNUSED(width);
    UNUSED(height);

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

CAMLprim value revery_HWND_getWidth(value vHWND) {
    CAMLparam1(vHWND);
    CAMLlocal1(vWidth);

    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);

    RECT rect;
    GetClientRect(
        hwnd,
        &rect
    );

    vWidth = Val_int(WIDTH(rect));

    CAMLreturn(vWidth);
}

CAMLprim value revery_HWND_getHeight(value vHWND) {
    CAMLparam1(vHWND);
    CAMLlocal1(vHeight);

    HWND hwnd = (HWND)revery_unwrapPointer(vHWND);

    RECT rect;
    GetClientRect(
        hwnd,
        &rect
    );

    vHeight = Val_int(HEIGHT(rect));

    CAMLreturn(vHeight);
}
#endif
