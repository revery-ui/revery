#include "config.h"
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

CAMLprim value revery_NSView_setFrame(value vNSView, value vX, value vY, value vWidth, value vHeight) {
    CAMLparam5(vNSView, vX, vY, vWidth, vHeight);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);
    int x = Int_val(vX);
    int y = Int_val(vY);
    int width = Int_val(vWidth);
    int height = Int_val(vHeight);

    CGRect frame = [nsView frame];
    frame.size.width = (CGFloat)width;
    frame.size.height = (CGFloat)height;

    [nsView setReveryX:(CGFloat)x];
    [nsView setReveryY:(CGFloat)y];

    [nsView updateFrame:frame];

    CAMLreturn(Val_unit);
}

CAMLprim value revery_NSView_displayIn(value vNSView, value vNSWindow) {
    CAMLparam2(vNSWindow, vNSView);
    NSWindow *nsWindow = (NSWindow *)revery_unwrapPointer(vNSWindow);
    NSView *nsView = (NSView *)revery_unwrapPointer(vNSView);

    NSView *contentView = [nsWindow contentView];
    CGRect frame = [nsView frame];
    [contentView addSubview:nsView];
    [nsView updateFrame: frame];

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
