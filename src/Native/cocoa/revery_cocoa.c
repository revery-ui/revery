#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#import "ReveryAppDelegate.h"

CAMLprim value revery_cocoaSetAppDelegate() {
  ReveryAppDelegate *delegate = [ReveryAppDelegate new];
  [NSApp setDelegate:delegate];
  return Val_unit;
}