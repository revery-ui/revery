#include "config.h"
#ifdef USE_COCOA
#include "ReveryAppDelegate_func.h"

#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>
#include <caml/alloc.h>

#include "caml_values.h"

#include "utilities.h"

#import <Cocoa/Cocoa.h>


/* revery_appDelegate_openFile
  This must be two functions:
    - one that acquires the CAML runtime
    - one that uses the CAML macros for proper memory management
  If you are implementing any functions similar to this, please
  keep this same structure! It is to prevent segmentation faults/
  other memory errors on runtime.
 */
CAMLprim value _revery_appDelegate_openFile(const char *path) {
    CAMLparam0();
    CAMLlocal1(vPath);

    static const value *dispatchFileOpen = NULL;
    if (dispatchFileOpen == NULL) {
        dispatchFileOpen = caml_named_value("revery_dispatchFileOpen");
    }
    // Call only if the value was gotten
    if (dispatchFileOpen != NULL) {
        vPath = caml_copy_string(path);
        caml_callback(*dispatchFileOpen, vPath);
    }
    CAMLreturn(Val_unit);
}
void revery_appDelegate_openFile(const char *path) {
    caml_c_thread_register();
    caml_acquire_runtime_system();
    _revery_appDelegate_openFile(path);
    caml_release_runtime_system();
}

#endif
