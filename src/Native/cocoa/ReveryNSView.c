#import "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>
#import "ReveryNSViewCoords.h"

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "utilities.h"

CAMLprim value revery_NSView_remove(value vNSView) {
    CAMLparam1(vNSView);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    [nsView removeFromSuperview];

    CAMLreturn(Val_unit);
}

CAMLprim value revery_NSView_setX(value vNSView, value vX) {
    CAMLparam2(vNSView, vX);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);
    int x = Int_val(vX);

    [nsView setReveryX:x];
    [nsView updateFrame];

    CAMLreturn(Val_unit);
}

CAMLprim value revery_NSView_setY(value vNSView, value vY) {
    CAMLparam2(vNSView, vY);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);
    int y = Int_val(vY);

    [nsView setReveryY:y];
    [nsView updateFrame];

    CAMLreturn(Val_unit);
}

CAMLprim value revery_NSView_getX(value vNSView) {
    CAMLparam1(vNSView);
    CAMLlocal1(vX);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    int x = (int)[nsView reveryX];
    vX = Val_int(x);

    CAMLreturn(vX);
}

CAMLprim value revery_NSView_getY(value vNSView) {
    CAMLparam1(vNSView);
    CAMLlocal1(vY);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    int y = (int)[nsView reveryY];
    vY = Val_int(y);

    CAMLreturn(vY);
}

CAMLprim value revery_NSView_setWidth(value vNSView, value vWidth) {
    CAMLparam2(vNSView, vWidth);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);
    int width = Int_val(vWidth);

    CGRect frame = [nsView frame];
    frame.size.width = (CGFloat)width;

    [nsView setFrame:frame];

    CAMLreturn(Val_unit);
}

CAMLprim value revery_NSView_setHeight(value vNSView, value vHeight) {
    CAMLparam2(vNSView, vHeight);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);
    int height = Int_val(vHeight);

    CGRect frame = [nsView frame];
    frame.size.height = (CGFloat)height;

    [nsView setFrame:frame];

    CAMLreturn(Val_unit);
}

CAMLprim value revery_NSView_getWidth(value vNSView) {
    CAMLparam1(vNSView);
    CAMLlocal1(vWidth);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    CGRect frame = [nsView frame];
    vWidth = Val_int((int)frame.size.width);

    CAMLreturn(vWidth);
}

CAMLprim value revery_NSView_getHeight(value vNSView) {
    CAMLparam1(vNSView);
    CAMLlocal1(vHeight);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    CGRect frame = [nsView frame];
    vHeight = Val_int((int)frame.size.height);

    CAMLreturn(vHeight);
}

CAMLprim value revery_NSView_displayIn(value vNSView, value vNSWindow) {
    CAMLparam2(vNSWindow, vNSView);
    NSWindow *nsWindow = (NSWindow *)revery_unwrapPointer(vNSWindow);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    NSView *contentView = [nsWindow contentView];
    [contentView addSubview:nsView];
    [nsView updateFrame];

    CAMLreturn(Val_unit);
}

CAMLprim value revery_NSView_getDefaultWidth(value vNSView) {
    CAMLparam1(vNSView);
    CAMLlocal1(vDefaultWidth);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    CGSize defaultSize = [nsView fittingSize];
    vDefaultWidth = Val_int((int)defaultSize.width);

    CAMLreturn(vDefaultWidth);
}

CAMLprim value revery_NSView_getDefaultHeight(value vNSView) {
    CAMLparam1(vNSView);
    CAMLlocal1(vDefaultHeight);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    CGSize defaultSize = [nsView fittingSize];
    vDefaultHeight = Val_int((int)defaultSize.height);

    CAMLreturn(vDefaultHeight);
}

#endif
