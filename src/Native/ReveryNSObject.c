#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#import <objc/objc-runtime.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

CAMLprim value revery_NSObject_equal(value vNSObjA, value vNSObjB) {
    CAMLparam2(vNSObjA, vNSObjB);
    NSObject *nsObjA = (NSObject *)vNSObjA;
    NSObject *nsObjB = (NSObject *)vNSObjB;

    BOOL equal = [nsObjA isEqual:nsObjB];

    CAMLreturn(Val_bool(equal));
}

CAMLprim value revery_NSObject_hash(value vNSObj) {
    CAMLparam1(vNSObj);
    NSObject *nsObj = (NSObject *)vNSObj;

    NSUInteger hash = [nsObj hash];

    CAMLreturn(Val_int(hash));
}

CAMLprim value revery_NSObject_toString(value vNSObj) {
    CAMLparam1(vNSObj);
    CAMLlocal1(vDescription);

    NSObject *nsObj = (NSObject *)vNSObj;
    NSString *nsDescription = [nsObj description];
    vDescription = caml_copy_string([nsDescription UTF8String]);

    CAMLreturn(vDescription);
}

CAMLprim value revery_NSObject_className(value vNSObj) {
    CAMLparam1(vNSObj);
    CAMLlocal1(vClassName);

    NSObject *nsObj = (NSObject *)vNSObj;
    NSString *nsClassName = NSStringFromClass([nsObj class]);
    vClassName = caml_copy_string([nsClassName UTF8String]);

    [nsClassName release];
    CAMLreturn(vClassName);
}

#endif
