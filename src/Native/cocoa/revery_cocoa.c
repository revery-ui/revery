#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#ifdef __APPLE__
#import "ReveryAppDelegate.h"
#endif

CAMLprim value revery_cocoaSetAppDelegate() {
#ifdef __APPLE__
  ReveryAppDelegate *delegate = [ReveryAppDelegate new];
  [NSApp setDelegate:delegate];
#endif
  return Val_unit;
}